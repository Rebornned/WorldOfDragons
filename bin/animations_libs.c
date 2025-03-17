#include <gtk/gtk.h>
#include <cairo.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "dlibs.h"

void startAnimation(GtkWidget *widget, gint animationIndex, gint totalFrames) {
    AnimationData *animData = g_malloc(sizeof(AnimationData));
    animData->widget = widget;
    animData->animationIndex = animationIndex;
    animData->currentFrame = 0;
    animData->totalFrames = totalFrames;

    g_signal_connect(widget, "draw", G_CALLBACK(on_draw_animation), animData);
    gtk_widget_queue_draw(widget);
}

gboolean delayedStartAnimation(gpointer data) {
    AnimationData *animData = (AnimationData *) data;
    startAnimation(animData->widget, animData->animationIndex, animData->totalFrames);
    g_free(animData);
    return FALSE;
}

void settingTimedVideoPlay(GtkWidget *widget, gint timeout, gint totalFrames, gchar *animationName) {
    gint animationIndex = -1;
    
    if (g_strcmp0(animationName, "battle_opening") == 0) {
        animationIndex = 1;
    } else if (g_strcmp0(animationName, "battle_opening2") == 0) {
        animationIndex = 2;
    }

    if (animationIndex == -1) {
        g_print("Erro: Nome da animação inválido!\n");
        return;
    }

    AnimationData *animData = g_malloc(sizeof(AnimationData));
    animData->widget = widget;
    animData->animationIndex = animationIndex;
    animData->totalFrames = totalFrames;
    
    g_timeout_add(timeout, delayedStartAnimation, animData);
}
