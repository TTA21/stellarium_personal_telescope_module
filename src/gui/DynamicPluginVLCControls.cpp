// At the top of your .cpp file, add:
#include "DynamicPluginTemplateWindow.hpp"
#include <vlc/vlc.h>
#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

void DynamicPluginTemplateWindow::initVLC()
{
    // Create VLC instance with better options for RTSP and embedding
    const char *vlc_argv[] = {
        "--no-xlib",  // Don't use Xlib for threading
        "--rtsp-tcp",  // Use TCP for RTSP
        "--network-caching=150",  // Lower caching for less latency
        "--rtsp-frame-buffer-size=500000",  // Increase buffer for RTCP issues
        "--no-audio",  // Disable audio
        "--avcodec-hw=none",  // Disable hardware acceleration (can cause issues)
        "--verbose=2"  // Add some verbosity for debugging
    };
    int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

    vlcInstance = libvlc_new(vlc_argc, vlc_argv);

    if (!vlcInstance)
    {
        qDebug() << "Failed to create VLC instance";
        return;
    }

    qDebug() << "VLC Initialized";

    // IMPORTANT: Widget must exist and be visible FIRST
    if (!videoWidget)
    {
        qDebug() << "videoWidget is NULL!";
        return;
    }

    // Make sure widget is shown and has a valid window ID
    videoWidget->show();
    videoWidget->setAttribute(Qt::WA_NativeWindow);
    videoWidget->setAttribute(Qt::WA_PaintOnScreen);

    // Process events to ensure window is created
    QCoreApplication::processEvents();

    WId wid = videoWidget->winId();
    qDebug() << "Widget WId:" << wid;

    // Create media player
    vlcMediaPlayer = libvlc_media_player_new(vlcInstance);

    if (!vlcMediaPlayer)
    {
        qDebug() << "Failed to create VLC media player";
        return;
    }

    // Set the window IMMEDIATELY after creating the player, BEFORE any media
    qDebug() << "Setting VLC to widget with WId:" << wid;

#if defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd(vlcMediaPlayer, (void*)wid);
#elif defined(Q_OS_MAC)
    libvlc_media_player_set_nsobject(vlcMediaPlayer, (void*)wid);
#else  // Linux/Unix
    libvlc_media_player_set_xwindow(vlcMediaPlayer, wid);
#endif

    qDebug() << "VLC window set successfully";
}

void DynamicPluginTemplateWindow::cleanupVLC()
{
    if (vlcMediaPlayer)
    {
        libvlc_media_player_stop(vlcMediaPlayer);
        libvlc_media_player_release(vlcMediaPlayer);
        vlcMediaPlayer = nullptr;
    }

    if (vlcMedia)
    {
        libvlc_media_release(vlcMedia);
        vlcMedia = nullptr;
    }

    if (vlcInstance)
    {
        libvlc_release(vlcInstance);
        vlcInstance = nullptr;
    }
}

void DynamicPluginTemplateWindow::playStream(const QString &url)
{
    if (!vlcInstance || !vlcMediaPlayer)
    {
        qDebug() << "VLC not initialized";
        return;
    }

    qDebug() << "Playing stream:" << url;

    // Stop current media if playing
    if (vlcMedia)
    {
        libvlc_media_player_stop(vlcMediaPlayer);
        libvlc_media_release(vlcMedia);
        vlcMedia = nullptr;
    }

    // Create new media
    vlcMedia = libvlc_media_new_location(vlcInstance, url.toUtf8().constData());

    if (!vlcMedia)
    {
        qDebug() << "Failed to create media for URL:" << url;
        return;
    }

    // Add options - these are applied to the media, not the player
    // For flip operations similar to your ffplay command
    libvlc_media_add_option(vlcMedia, ":video-filter=transform");
    libvlc_media_add_option(vlcMedia, ":transform-type=vflip");
    libvlc_media_add_option(vlcMedia, ":transform-type=hflip");

    // Additional options for better RTSP handling
    libvlc_media_add_option(vlcMedia, ":rtsp-tcp");
    libvlc_media_add_option(vlcMedia, ":network-caching=150");

    // Set media to player
    libvlc_media_player_set_media(vlcMediaPlayer, vlcMedia);

    // Verify widget is set before playing
    if (videoWidget && videoWidget->isVisible())
    {
        qDebug() << "Starting playback...";
        int result = libvlc_media_player_play(vlcMediaPlayer);

        if (result == -1)
        {
            qDebug() << "Failed to start playback";
        }
        else
        {
            qDebug() << "Playback started successfully";
        }
    }
    else
    {
        qDebug() << "videoWidget not visible or null!";
    }
}

void DynamicPluginTemplateWindow::stopStream()
{
    if (vlcMediaPlayer)
    {
        libvlc_media_player_stop(vlcMediaPlayer);
    }
}
