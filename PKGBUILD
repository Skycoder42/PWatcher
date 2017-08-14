# Maintainer: Felix Barz <skycoder42.de@gmx.de>
pkgname=pwatcher
pkgver=1.1.0
pkgrel=3
pkgdesc="P-Watcher Diashow Tool"
arch=('i686' 'x86_64')
url="https://github.com/Skycoder42/$pkgname"
license=('BSD')
depends=('qt5-base' 'qt5-quickcontrols2')
makedepends=('git' 'qpm' 'qt5-tools')
optdepends=('qt5-imageformats: Support for additional icon types'
			'qt5-svg: Support for SVG-Icons'
			'qt5-apng-plugin: Support for APNG-Files')
_pkgfqn=$pkgname-$pkgver
source=("$_pkgfqn::git+https://github.com/Skycoder42/$pkgname.git#branch=rebuild") #tag=$pkgver")
sha256sums=('SKIP')

prepare() {
  mkdir -p build
  
  cd "$_pkgfqn"
  qpm install
}

build() {
  cd build

  qmake -r "../$_pkgfqn/"
  make
}

package() {
  cd build
  make INSTALL_ROOT="$pkgdir" install

  cd "../$_pkgfqn/"
  install -D -m644 de.skycoder42.${pkgname}.desktop "$pkgdir/usr/share/applications/de.skycoder42.${pkgname}.desktop"
  install -D -m644 ${pkgname}_32.png "$pkgdir/usr/share/icons/hicolor/32x32/apps/$pkgname.png"
  install -D -m644 ${pkgname}_16.png "$pkgdir/usr/share/icons/hicolor/16x16/apps/$pkgname.png"
  install -D -m644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
}
