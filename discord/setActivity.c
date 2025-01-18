#include <stdio.h>
#include <assert.h>
#include "discord_game_sdk.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <string.h>
#include "setActivity.h"
#endif

#define DISCORD_REQUIRE(x) assert(x == DiscordResult_Ok)

struct Application
{
    struct IDiscordCore *core;
    struct IDiscordUserManager *users;
    struct IDiscordAchievementManager *achievements;
    struct IDiscordActivityManager *activities;
    struct IDiscordRelationshipManager *relationships;
    struct IDiscordApplicationManager *application;
    struct IDiscordLobbyManager *lobbies;
    DiscordUserId user_id;
};

void DISCORD_CALLBACK logHook(void *data, enum EDiscordLogLevel level, const char *message)
{
    // Your logging implementation here
    FILE *file = fopen("log.txt", "w");
    if (file != NULL)
    {
        fprintf(file, "Result: %s\n", message);
        fclose(file);
    }
    else
    {
        perror("Failed to open file");
    }
    printf("Log level: %d, Message: %s\n", level, message);
}

void DISCORD_CALLBACK UpdateActivityCallback(void *data, enum EDiscordResult result)
{
    // FILE *file = fopen("log.txt", "w");
    // if (file != NULL)
    // {
    //     fprintf(file, "Result: %d\n", result);
    //     fclose(file);
    // }
    // else
    // {
    //     perror("Failed to open file");
    // }
    DISCORD_REQUIRE(result);
}

void DISCORD_CALLBACK OnOAuth2Token(void *data,
                                    enum EDiscordResult result,
                                    struct DiscordOAuth2Token *token)
{
    if (result == DiscordResult_Ok)
    {
        printf("OAuth2 token: %s\n", token->access_token);
    }
    else
    {
        printf("GetOAuth2Token failed with %d\n", (int)result);
    }
}

void setActivity(char *filename, char *language)
{
    struct Application app;
    struct DiscordActivity activity;
    memset(&app, 0, sizeof(app));
    memset(&activity, 0, sizeof(activity)); // Initialize activity structure
    struct IDiscordActivityEvents activities_events;
    memset(&activities_events, 0, sizeof(activities_events));
    struct DiscordCreateParams params;
    DiscordCreateParamsSetDefault(&params);
    params.client_id = 1330215173864296450;
    params.activity_events = &activities_events;
    DISCORD_REQUIRE(DiscordCreate(DISCORD_VERSION, &params, &app.core));

    app.activities = app.core->get_activity_manager(app.core);
    app.application = app.core->get_application_manager(app.core);
    app.core->set_log_hook(app.core, DiscordLogLevel_Debug, NULL, logHook);
    app.application->get_oauth2_token(app.application, &app, OnOAuth2Token);
    app.relationships = app.core->get_relationship_manager(app.core);
    sprintf(activity.details, "%s", filename);
    sprintf(activity.assets.large_image, "%s", "helix_logo");

    // Initialize party and size fields
    activity.party.size.current_size = 1; // Set to a valid value
    activity.party.size.max_size = 10; // Example max size

    FILE *file = fopen("activity.txt", "w");
    if (file != NULL)
    {
        fprintf(file, "Filename: %s\n", filename);
        fprintf(file, "Language: %s\n", language);
        fclose(file);
    }
    else
    {
        perror("Failed to open file");
    }
    app.activities->update_activity(app.activities, &activity, &app, UpdateActivityCallback);
    for (;;)
    {
        DISCORD_REQUIRE(app.core->run_callbacks(app.core));

#ifdef _WIN32
        Sleep(16);
#else
        usleep(16 * 1000);
#endif
    }
    return;
}

int main(int argc, char **argv)
{
    setActivity("Hello, World!", "C");
}

