// This is copyrighted software. More information is at the end of this file.
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <cstdlib>

#include "happlication.h"
extern "C" {
#include "heheader.h"
}
#include "hugodefs.h"
#include "settings.h"

// On some platforms, SDL redefines main in order to provide a platform-specific main()
// implementation. However, Qt handles this too, so things can get weird. We need to make sure main
// is not redefined so that Qt can find our own implementation and SDL will not try to do
// platform-specific initialization work (like launching the Cocoa event-loop or setting up the
// application menu on OS X, or redirecting stdout and stderr to text files on Windows), which would
// break things.
#ifdef main
#undef main
#endif

int main(int argc, char* argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    HApplication app(argc, argv, "Hugor", HUGOR_VERSION, "Nikos Chantziaras", "");
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    initSoundEngine();
    initVideoEngine(argc, argv);

    // Filename of the game to run.
    QString gameFileName;

    // Check if a game file with the same basename as ours exists in our directory.  If yes, we will
    // run it.
    gameFileName = HApplication::applicationDirPath();
    if (not gameFileName.endsWith('/')) {
        gameFileName += '/';
    }
    gameFileName += QFileInfo(HApplication::applicationFilePath()).baseName();
    gameFileName += ".hex";
    if (not QFileInfo::exists(gameFileName)) {
        gameFileName.clear();
    }

    const QStringList& args = HApplication::arguments();
    if (args.size() == 2) {
        if (QFile::exists(args.at(1))) {
            gameFileName = args.at(1);
        } else if (QFile::exists(args.at(1) + QString::fromLatin1(".hex"))) {
            gameFileName = args.at(1) + QString::fromLatin1(".hex");
        } else {
            qWarning() << "File" << args.at(1) << "not found.";
        }
    }

    int ret = 0;
    QTimer::singleShot(0, &app, [&app, gameFileName] { app.entryPoint(gameFileName); });
    ret = HApplication::exec();
    closeVideoEngine();
    closeSoundEngine();
    return ret;
}

/* Copyright (C) 2011-2019 Nikos Chantziaras
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
