// This is copyrighted software. More information is at the end of this file.
#pragma once
#include <QWidget>

#include <gst/app/gstappsrc.h>
#include <gst/gstelement.h>
#include <gst/gstpipeline.h>

class VideoPlayer;

class VideoPlayer_priv final: public QWidget
{
    Q_OBJECT

public:
    VideoPlayer_priv(QWidget* parent, VideoPlayer* qPtr);
    VideoPlayer* q;
    GstElement* pipeline = nullptr;
    GstAppSrc* appsrc = nullptr;
    GstBus* bus = nullptr;
    GstAppSrcCallbacks appsrc_cbs;
    int volume = 100;

    static GMainLoop* main_loop;
    static GThread* main_loop_thread;

    static void cbOnSourceSetup(GstAppSrc* source, VideoPlayer_priv* d);
    static void cbOnBusMessage(GstMessage* message, VideoPlayer_priv* d);

public slots:
    void adjustForVideoSize(QSize vidSize);
};

/* Copyright (C) 2011-2018 Nikos Chantziaras
 *
 * This file is part of Hugor.
 *
 * Hugor is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Hugor is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Hugor.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Additional permission under GNU GPL version 3 section 7
 *
 * If you modify this Program, or any covered work, by linking or combining it
 * with the Hugo Engine (or a modified version of the Hugo Engine), containing
 * parts covered by the terms of the Hugo License, the licensors of this
 * Program grant you additional permission to convey the resulting work.
 * Corresponding Source for a non-source form of such a combination shall
 * include the source code for the parts of the Hugo Engine used as well as
 * that of the covered work.
 */
