(defpackage dabase.builder
  (:use :common-lisp)
  (:export build))
(in-package :dabase.builder)

(deftype node () '(unsigned-byte 32))

(defconstant +EMPTY+ 0)

#|
DA & NODE
|#
(defstruct da
  (nodes #() :type (simple-array node)))

(defun init-da ()
  (make-da :nodes (make-array 64 :element-type 'node :initial-element +EMPTY+)))

(defun ensure-index (da index)
  (with-slots (nodes) (the da da)
    (when (<= (length nodes) index)
      (setf nodes (adjust-array nodes (* (length nodes) 2) :initial-element +EMPTY+))
      (ensure-index da index))))

(defun node-ref (da index)
  (ensure-index da index)
  (aref (da-nodes da) index))

(defun (setf node-ref) (node da index)
  (ensure-index da index)
  (setf (aref (da-nodes da) index) node))

(defun build (entries output-file)
  (let ((da (init-da)))
    (build-impl da 0 (length entries) 0)
    (output-to-file da output-file))
  :done)


#|
BUILD
|#
(defun build-impl (da beg end root-node) 
  )

#|
OUTPUT
|#
(defun output-to-file (da output-file)
  (declare (ignore da output-file))
  )
