#include <gtk/gtk.h>
#include <cairo.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "dlibs.h"

void startAnimation(GtkWidget *widget, gint animationIndex, gint totalFrames, gint isLoop, gint *cancelAnimation, gchar *animationName) {
    AnimationData *animData = g_malloc(sizeof(AnimationData));
    animData->widget = widget;
    animData->animationIndex = animationIndex;
    animData->currentFrame = 0;
    animData->totalFrames = totalFrames;
    animData->isLoop = isLoop;
    animData->cancelAnimation = cancelAnimation;
    animData->finished = FALSE;
    animData->startTime = g_get_monotonic_time();  // Armazena tempo inicial
    strcpy(animData->animationName, animationName);

    g_signal_connect(widget, "draw", G_CALLBACK(on_draw_animation), animData);
    gtk_widget_queue_draw(widget);
}

gboolean delayedStartAnimation(gpointer data) {
    AnimationData *animData = (AnimationData *) data;
    startAnimation(animData->widget, animData->animationIndex, animData->totalFrames, animData->isLoop, animData->cancelAnimation, animData->animationName);
    g_free(animData);
    return FALSE;
}

void settingTimedVideoPlay(GtkWidget *widget, gint timeout, gint totalFrames, gchar *animationName, gint isLoop, gint *cancelAnimation) {
    gint animationIndex = -1;
    
    if (g_strcmp0(animationName, "battle_opening") == 0) animationIndex = 1;
    else if (g_strcmp0(animationName, "battle_opening2") == 0) animationIndex = 2;
    else if (g_strcmp0(animationName, "bleeding_status_apply") == 0) animationIndex = 3;
    else if (g_strcmp0(animationName, "bleeding_status_finish") == 0) animationIndex = 4;
    else if (g_strcmp0(animationName, "broken_status_apply") == 0) animationIndex = 5;
    else if (g_strcmp0(animationName, "broken_status_finish") == 0) animationIndex = 6;
    else if (g_strcmp0(animationName, "burning_status_apply") == 0) animationIndex = 7;
    else if (g_strcmp0(animationName, "burning_status_finish") == 0) animationIndex = 8;
    else if (g_strcmp0(animationName, "terrified_status_apply") == 0) animationIndex = 9;
    else if (g_strcmp0(animationName, "terrified_status_finish") == 0) animationIndex = 10;
    else if (g_strcmp0(animationName, "victory") == 0) animationIndex = 11;
    else if (g_strcmp0(animationName, "defeat") == 0) animationIndex = 12;
    else if (g_strcmp0(animationName, "keypress") == 0) animationIndex = 13;

    if (animationIndex == -1) {
        g_print("Erro: Nome da animação inválido!\n");
        return;
    }

    AnimationData *animData = g_malloc(sizeof(AnimationData));
    animData->widget = widget;
    animData->animationIndex = animationIndex;
    animData->totalFrames = totalFrames;
    animData->isLoop = isLoop;
    animData->cancelAnimation = cancelAnimation;
    strcpy(animData->animationName, animationName);
    
    g_timeout_add(timeout, delayedStartAnimation, animData);
}
