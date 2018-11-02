# Maintainer: Jack Sorrell <jack@jacksorrell.com>

pkgname=plasma-browser-integration-stdin-patch
pkgver=5.14.2
pkgrel=1
pkgdesc='Components necessary to integrate browsers into the Plasma Desktop - Patched to work despite QT bugs.'
arch=(x86_64)
url='https://www.kde.org/plasma-desktop'
license=(GPL)
provides=('plasma-browser-integration')
conflicts=('plasma-browser-integration')
depends=(krunner)
makedepends=(extra-cmake-modules)
groups=(plasma)
source=("https://download.kde.org/stable/plasma/${pkgver}/plasma-browser-integration-${pkgver}.tar.xz"{,.sig}
    'connection-noqt.cpp'
    'connection-noqt.h')
sha256sums=('4ff64139dcb3420d5a90d1b182a901e43e6527896a227f200896dbb099f61e75'
            'SKIP'
            'SKIP'
            'SKIP')
validpgpkeys=('2D1D5B0588357787DE9EE225EC94D18F7F05997E'  # Jonathan Riddell <jr@jriddell.org>
              '0AAC775BB6437A8D9AF7A3ACFE0784117FBCE11D'  # Bhushan Shah <bshah@kde.org>
              'D07BD8662C56CB291B316EB2F5675605C74E02CF'  # David Edmundson <davidedmundson@kde.org>
              '1FA881591C26B276D7A5518EEAAF29B42A678C20') # Marco Martin <notmart@gmail.com>

prepare() {
    cd "${srcdir}"
    msg "Replace connection implementation"
    cp "connection-noqt.cpp" "plasma-browser-integration-${pkgver}/host/connection.cpp"
    cp "connection-noqt.h" "plasma-browser-integration-${pkgver}/host/connection.h"

    mkdir -p build
}

build() {
  cd "${srcdir}/build"
  cmake "../plasma-browser-integration-${pkgver}" \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DCMAKE_INSTALL_LIBDIR=lib \
    -DINSTALL_CHROME_MANIFEST=ON
  make
}

package() {
  cd build
  make DESTDIR="$pkgdir" install
}
