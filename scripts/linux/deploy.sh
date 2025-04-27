#!/bin/bash

export QML_SOURCES_PATHS="$(pwd)/src/ui"

APP_NAME="KhinsiderQT"
APP_EXECUTABLE="build/appKhinsiderQT"
ICON_PATH="icons/icon.png"
DEPLOY_DIR="deploy"
RESULT_DIR="$DEPLOY_DIR/result"
LINUXDEPLOY_URL="https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage"
LINUXDEPLOY_BIN="$DEPLOY_DIR/linuxdeploy-x86_64.AppImage"
PLUGIN_QT_URL="https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage"
PLUGIN_QT_BIN="$DEPLOY_DIR/linuxdeploy-plugin-qt-x86_64.AppImage"

mkdir -p $DEPLOY_DIR $RESULT_DIR

echo "Downloading linuxdeploy..."
if [ ! -f "$LINUXDEPLOY_BIN" ]; then
    wget -q $LINUXDEPLOY_URL -O $LINUXDEPLOY_BIN
    chmod +x $LINUXDEPLOY_BIN
fi


echo "Downloading linuxdeploy Qt plugin..."
if [ ! -f "$PLUGIN_QT_BIN" ]; then
    wget -q $PLUGIN_QT_URL -O $PLUGIN_QT_BIN
    chmod +x $PLUGIN_QT_BIN
fi

echo "Creating .desktop file..."
cat > "$DEPLOY_DIR/$APP_NAME.desktop" << EOF
[Desktop Entry]
Type=Application
Name=$APP_NAME
Comment=Khinsider OST downloader
Exec=appKhinsiderQT
Icon=icon
Categories=AudioVideo;Audio;Music;
EOF

echo "Copying application executable..."
if [ -f "$APP_EXECUTABLE" ]; then
    cp "$APP_EXECUTABLE" "$DEPLOY_DIR/"
else
    echo "Error: Executable not found at $APP_EXECUTABLE"
    exit 1
fi

echo "Copying icon..."
if [ -f "$ICON_PATH" ]; then

    mkdir -p "$DEPLOY_DIR/usr/share/icons/hicolor/256x256/apps/"
    mkdir -p "$DEPLOY_DIR/usr/share/icons/hicolor/128x128/apps/"
    mkdir -p "$DEPLOY_DIR/usr/share/icons/hicolor/64x64/apps/"

    cp "$ICON_PATH" "$DEPLOY_DIR/icon.png"
    cp "$ICON_PATH" "$DEPLOY_DIR/usr/share/icons/hicolor/256x256/apps/icon.png"
    cp "$ICON_PATH" "$DEPLOY_DIR/usr/share/icons/hicolor/128x128/apps/icon.png"
    cp "$ICON_PATH" "$DEPLOY_DIR/usr/share/icons/hicolor/64x64/apps/icon.png"
else
    echo "Error: Icon not found at $ICON_PATH"
    exit 1
fi

echo "Running linuxdeploy..."
cd $DEPLOY_DIR
./linuxdeploy-x86_64.AppImage \
    --executable "./appKhinsiderQT" \
    --desktop-file "./$APP_NAME.desktop" \
    --icon-file "./icon.png" \
    --appdir ./AppDir \
    --output appimage \
    --plugin qt

echo "Moving AppImage to result directory..."
mkdir -p ./result
mv ./${APP_NAME}*.AppImage ./result/

echo "Deployment complete! AppImage is available in $RESULT_DIR/"
