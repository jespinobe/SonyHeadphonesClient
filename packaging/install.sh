#!/bin/bash

set -e

PROJECT="$HOME/SonyHeadphonesClient"
BUILD="$PROJECT/Client/build"
INSTALL_DIR="/opt/sony-headphones-client"

APP_NAME="Sony Headphones Client"
BIN_NAME="SonyHeadphonesClient"

echo "== Instalando $APP_NAME =="

# Verificar binario
if [ ! -f "$BUILD/$BIN_NAME" ]; then
    echo "ERROR: no existe:"
    echo "$BUILD/$BIN_NAME"
    echo ""
    echo "Compila primero:"
    echo "cd $BUILD"
    echo "cmake --build . -j\$(nproc)"
    exit 1
fi

echo "Creando directorio..."

sudo mkdir -p "$INSTALL_DIR"

echo "Copiando ejecutable..."

sudo cp \
"$BUILD/$BIN_NAME" \
"$INSTALL_DIR/"

sudo chmod +x \
"$INSTALL_DIR/$BIN_NAME"

echo "Buscando icono..."

ICON=$(find "$PROJECT" \
-type f \
\( -iname "*.png" \
-o -iname "*.svg" \
-o -iname "*.jpg" \) \
| head -n1)

if [ -n "$ICON" ]; then

    EXT="${ICON##*.}"

    sudo cp \
    "$ICON" \
    "$INSTALL_DIR/icon.$EXT"

    ICON_PATH="$INSTALL_DIR/icon.$EXT"

    echo "Icono encontrado:"
    echo "$ICON"

else

    ICON_PATH="utilities-terminal"

    echo "No encontré icono."
    echo "Usando icono del sistema."

fi

echo "Creando acceso menú..."

cat > /tmp/sony-headphones.desktop <<EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=$APP_NAME
Comment=Unofficial Sony WH Headphones Client
Exec=$INSTALL_DIR/$BIN_NAME
Icon=$ICON_PATH
Terminal=false
Categories=Audio;Utility;
StartupNotify=true
EOF

mkdir -p ~/.local/share/applications

cp \
/tmp/sony-headphones.desktop \
~/.local/share/applications/

chmod +x \
~/.local/share/applications/sony-headphones.desktop

update-desktop-database \
~/.local/share/applications \
2>/dev/null || true

echo ""
echo "Instalación completada"
echo ""
echo "Ahora busca:"
echo ""
echo "Sony Headphones Client"
echo ""
echo "en el menú de aplicaciones."