#include <gtk/gtk.h>
#include <cairo.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "dlibs.h"

typedef struct {
    GtkWindow *window;
    gint original_x, original_y;
    gint duration;
    gint intensity;
    gint64 start_time;
    guint timeout_id;
} shakeAnimationData;

void startAnimation(GtkWidget *widget, gint animationIndex, gint totalFrames, gint isLoop, gint *cancelAnimation, gchar *animationName, gboolean canDestroy) {
    AnimationData *animData = g_malloc(sizeof(AnimationData));
    animData->widget = widget;
    animData->animationIndex = animationIndex;
    animData->currentFrame = 0;
    animData->totalFrames = totalFrames;
    animData->isLoop = isLoop;
    animData->cancelAnimation = cancelAnimation;
    animData->finished = FALSE;
    animData->canDestroy = canDestroy;
    animData->startTime = g_get_monotonic_time();  // Armazena tempo inicial
    strcpy(animData->animationName, animationName);

    g_signal_connect(widget, "draw", G_CALLBACK(on_draw_animation), animData);
    gtk_widget_queue_draw(widget);
}

gboolean delayedStartAnimation(gpointer data) {
    AnimationData *animData = (AnimationData *) data;
    startAnimation(animData->widget, animData->animationIndex, animData->totalFrames, animData->isLoop, animData->cancelAnimation, animData->animationName, animData->canDestroy);
    g_free(animData);
    return FALSE;
}

void settingTimedVideoPlay(GtkWidget *widget, gint timeout, gint totalFrames, gchar *animationName, gint isLoop, gint *cancelAnimation, gboolean canDestroy) {
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
    else if (g_strcmp0(animationName, "fire_breath_animation") == 0) animationIndex = 14;
    else if (g_strcmp0(animationName, "scratch_claw_animation_ent1") == 0) animationIndex = 15;
    else if (g_strcmp0(animationName, "bite_crunch_animation") == 0) animationIndex = 16;
    else if (g_strcmp0(animationName, "scratch_claw_animation_ent2") == 0) animationIndex = 17;
    else if (g_strcmp0(animationName, "dragon_grow_animation") == 0) animationIndex = 18;
    else if (g_strcmp0(animationName, "unstable_status_apply") == 0) animationIndex = 19;
    else if (g_strcmp0(animationName, "unstable_status_finish") == 0) animationIndex = 20;
    else if (g_strcmp0(animationName, "freezing_status_apply") == 0) animationIndex = 21;
    else if (g_strcmp0(animationName, "freezing_status_finish") == 0) animationIndex = 22;


    if (animationIndex == -1) {
        g_print("Erro: Nome da animação inválido!\n");
        return;
    }

    AnimationData *animData = g_malloc(sizeof(AnimationData));
    animData->widget = widget;
    animData->animationIndex = animationIndex;
    animData->totalFrames = totalFrames;
    animData->isLoop = isLoop;
    animData->canDestroy = canDestroy;
    animData->cancelAnimation = cancelAnimation;
    strcpy(animData->animationName, animationName);
    
    g_timeout_add(timeout, delayedStartAnimation, animData);
}

void settingTimedNewWidgetAnimation(gint timeout, gint totalFrames, gchar *animationName, GtkFixed *fixed, gint posX, gint posY, gint width, gint height) {
    if(timeout == 0) {
        GtkWidget *widget = gtk_drawing_area_new();
        gtk_fixed_put(GTK_FIXED(fixed), widget, posX, posY); 
        gtk_widget_set_size_request(widget, width, height);     
        gtk_widget_show(widget);
        gtk_widget_realize(widget);
        gtk_widget_queue_draw(widget);
        settingTimedVideoPlay(widget, 0, totalFrames, animationName, 0, NULL, TRUE);
    }
    else {
        WidgetAnimationData *animData = g_malloc(sizeof(WidgetAnimationData));
        animData->timeout = 0;
        animData->totalFrames = totalFrames;
        animData->animationName = g_strdup(animationName);
        animData->fixed = fixed;
        animData->posX = posX;
        animData->posY = posY;
        animData->width = width;
        animData->height = height;
        g_timeout_add(timeout, delayedNewWidgetAnimation, animData);
    }
}

gboolean delayedNewWidgetAnimation(gpointer data) {
    WidgetAnimationData *aD = (WidgetAnimationData *) data;
    settingTimedNewWidgetAnimation(aD->timeout, aD->totalFrames, aD->animationName, aD->fixed, aD->posX, aD->posY, aD->width, aD->height);
    g_free(aD);
    return FALSE;
}

void settingAttackAnimation(gint timeout, gint entityNumber, gint totalFrames, gchar *animationName, GtkFixed *fixed, gint size) {
    gint posX[10];
    gint posY[10];

    if(entityNumber == 1) {
        if(g_strcmp0(animationName, "fire_breath_animation") == 0) {
            posX[0] = 664; posX[1] = 770;
            posY[0] = 110; posY[1] = 193; posY[2] = 258; posY[3] = 324;
        }
        if(g_strcmp0(animationName, "scratch_claw_animation") == 0) {
            posX[0] = random_choice(648, 710); posX[1] = random_choice(648, 710); posX[2] = random_choice(648, 710);
            posY[0] = random_choice(174, 260), posY[1] = random_choice(174, 260); posY[2] = random_choice(174, 260); 
        }
        if(g_strcmp0(animationName, "bite_crunch_animation") == 0) {
            posX[0] = random_choice(671, 762); posX[1] = random_choice(671, 762); posX[2] = random_choice(671, 762);
            posX[3] = random_choice(671, 762); posX[4] = random_choice(671, 762);
            posY[0] = random_choice(236, 280), posY[1] = random_choice(236, 280); posY[2] = random_choice(236, 280);
            posY[3] = random_choice(236, 280);  posY[4] = random_choice(236, 280); 
        }
    }
    else if(entityNumber == 2) {
        if(g_strcmp0(animationName, "fire_breath_animation") == 0) {
            posX[0] = 17; posX[1] = 124;
            posY[0] = 114; posY[1] = 193; posY[2] = 258; posY[3] = 324;
        }
        if(g_strcmp0(animationName, "scratch_claw_animation") == 0) {
            posX[0] = random_choice(40, 80); posX[1] = random_choice(40, 80); posX[2] = random_choice(40, 80);
            posY[0] = random_choice(174, 260), posY[1] = random_choice(174, 260); posY[2] = random_choice(174, 260); 
        }
        if(g_strcmp0(animationName, "bite_crunch_animation") == 0) {
            posX[0] = random_choice(80, 160); posX[1] = random_choice(80, 160); posX[2] = random_choice(80, 160);
            posX[3] = random_choice(80, 160); posX[4] = random_choice(80, 160);
            posY[0] = random_choice(236, 280), posY[1] = random_choice(236, 280); posY[2] = random_choice(236, 280);
            posY[3] = random_choice(236, 280);  posY[4] = random_choice(236, 280); 
        }
    }
    
    if(g_strcmp0(animationName, "fire_breath_animation") == 0) {
        for(int g=0; g<3; g++) {
            playSoundByName(timeout + 500*g, "dracarys_breath", &audioPointer, 0);
            for(int i=0; i < 2; i++)
                for(int j=0; j < 4; j++) 
                    settingTimedNewWidgetAnimation(timeout + (500*g), totalFrames, animationName, fixed, posX[i], posY[j], size, size);
        }
    }

    if(g_strcmp0(animationName, "scratch_claw_animation") == 0) {
        if(entityNumber == 1) animationName = g_strdup("scratch_claw_animation_ent1");
        else if(entityNumber == 2) animationName = g_strdup("scratch_claw_animation_ent2");

        for(int i=0; i<3; i++) {
            playSoundByName(timeout + 250*i, "scratch_claw", &audioPointer, 0);
            settingTimedNewWidgetAnimation(timeout + 250*i, totalFrames, animationName, fixed, posX[i], posY[i], size, size);
        }
    }

    if(g_strcmp0(animationName, "bite_crunch_animation") == 0) {
        for(int i=0; i<5; i++) {
            playSoundByName(timeout + 250*i, "bite_crunch", &audioPointer, 0);
            settingTimedNewWidgetAnimation(timeout + 250*i, totalFrames, animationName, fixed, posX[i]-40, posY[i]-40, size, size);
        }
    }

}

gboolean shakeAnimation(gpointer user_data) {
    shakeAnimationData *data = (shakeAnimationData *)user_data;
    gint64 now = g_get_monotonic_time(); 
    gint elapsed = (now - data->start_time) / 1000;

    if (elapsed > data->duration) {
        gtk_window_move(data->window, data->original_x, data->original_y);
        g_free(data);
        return FALSE;
    }

    gint dx = (rand() % (2 * data->intensity + 1)) - data->intensity;
    gint dy = (rand() % (2 * data->intensity + 1)) - data->intensity;
    gtk_window_move(data->window, data->original_x + dx, data->original_y + dy);

    return TRUE;
}

void shakeScreen(gint timeout, GtkWindow *window, gint duration, gint intensity) {
    shakeAnimationData *data = g_malloc(sizeof(shakeAnimationData));
    data->window = window;
    data->duration = duration;
    data->intensity = intensity;
    data->start_time = g_get_monotonic_time();
    
    gtk_window_get_position(window, &data->original_x, &data->original_y);
    
    if(timeout == 0) {
        g_timeout_add(20, shakeAnimation, data);
    }
    else
        g_timeout_add(timeout, timedShakeScreen, data);
    
}

gboolean timedShakeScreen(gpointer data) {
    shakeAnimationData *values = (shakeAnimationData *) data;
    shakeScreen(0, values->window, values->duration, values->intensity);
    g_free(values);

    return FALSE;
}
