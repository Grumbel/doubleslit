;;
;; Copyright (C) 2019 Ingo Ruhnke <grumbel@gmail.com>
;;
;; This program is free software: you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.

(set! %load-path
  (cons* "/ipfs/QmX6nVomBirTsHRVDiuQQgPsRWoS7HxcgFeAgwJWdGQ332/guix-cocfree_0.0.0-62-g3b27118"
         %load-path))

(use-modules (guix build-system cmake)
             ((guix licenses) #:prefix license:)
             (guix packages)
             (gnu packages gtk)
             (gnu packages maths)
             (gnu packages pkg-config)
             (guix-cocfree utils))

(define %source-dir (dirname (current-filename)))

(define-public doubleslit
  (package
   (name "doubleslit")
   (version (version-from-source %source-dir))
   (source (source-from-source %source-dir))
   (arguments
    `(#:tests? #f))
   (build-system cmake-build-system)
   (native-inputs
    `(("pkg-config" ,pkg-config)))
   (inputs
    `(("glm" ,glm)
      ("gtkmm" ,gtkmm)
      ("cairomm" ,cairomm)))
   (synopsis (synopsis-from-source %source-dir))
   (description (description-from-source %source-dir))
   (home-page (homepage-from-source %source-dir))
   (license license:gpl3+)))

doubleslit

;; EOF ;;

