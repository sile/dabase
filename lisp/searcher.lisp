(defpackage dabase.searcher
  (:use :common-lisp)
  (:shadow :common-lisp get load)
  (:export load
           member?
           get
           each-common-prefix))
(in-package :dabase.searcher)




