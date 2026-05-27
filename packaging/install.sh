#!/bin/bash
set -e

APP_NAME="SonyHeadphonesClient"
INSTALL_DIR="/opt/$APP_NAME"
ICON_NAME="sony-headphones"

PROJECT_ROOT="$(cd "$(dirname "$0")/.." && pwd)"

echo "=================================="
echo "Sony Headphones Client Installer"
echo "=================================="

echo ""
echo "[1/5] Compilando..."

cd "$PROJECT_ROOT/Client"

mkdir -p build
cd build

cmake .. -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build . -j$(nproc)

echo ""
echo "[2/5] Instalando / actualizando..."

sudo mkdir -p "$INSTALL_DIR"

sudo install -m755 \
SonyHeadphonesClient \
"$INSTALL_DIR/SonyHeadphonesClient"


echo ""
echo "[3/5] Instalando icono..."

if [ ! -f "$PROJECT_ROOT/static/sony-headphones.png" ]
then
    echo ""
    echo "ERROR:"
    echo "No existe:"
    echo "$PROJECT_ROOT/static/sony-headphones.png"
    exit 1
fi

sudo mkdir -p \
/usr/share/icons/hicolor/512x512/apps

sudo cp -f \
"$PROJECT_ROOT/static/sony-headphones.png" \
/usr/share/icons/hicolor/512x512/apps/$ICON_NAME.png

sudo cp -f \
"$PROJECT_ROOT/static/sony-headphones.png" \
/usr/share/pixmaps/$ICON_NAME.png


# eliminar restos viejos
rm -f ~/.local/share/icons/$ICON_NAME*
rm -f ~/.local/share/applications/sony-headphones.desktop

rm -rf ~/.cache/thumbnails/*
rm -rf ~/.cache/gnome-software/*
rm -rf ~/.cache/icon-cache.kcache*
rm -rf ~/.cache/*icon*


echo ""
echo "[4/5] Creando launcher..."

sudo tee \
/usr/share/applications/sony-headphones.desktop \
>/dev/null <<EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=Sony Headphones Client
Comment=Sony Headphones Desktop Client
Exec=$INSTALL_DIR/SonyHeadphonesClient
Icon=$ICON_NAME
Terminal=false
Categories=Audio;Utility;
StartupNotify=true
EOF


echo ""
echo "[5/5] Refrescando GNOME..."

sudo update-desktop-database \
/usr/share/applications \
2>/dev/null || true

sudo gtk-update-icon-cache \
-f \
-t \
/usr/share/icons/hicolor \
2>/dev/null || true

sudo touch /usr/share/icons/hicolor


# GNOME es muy agresivo cacheando iconos
#if pgrep -x gnome-shell >/dev/null
#then
#    echo "Reiniciando GNOME Shell..."
#    
#    killall gnome-shell \
#    2>/dev/null || true

#    nohup gnome-shell --replace \
#    >/dev/null 2>&1 &
#fi

echo ""
echo "=================================="
echo "Instalación completa"
echo "=================================="

echo ""
echo "Abrir desde:"
echo "Aplicaciones → Sony Headphones Client"