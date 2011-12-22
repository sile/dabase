(defpackage dabase.builder
  (:use :common-lisp)
  (:export build))
(in-package :dabase.builder)

(deftype node () '(unsigned-byte 32))

(eval-when (:compile-toplevel :load-toplevel)
  (rename-package :dabase.octet-stream :dabase.octet-stream '(:octet-stream))
  (rename-package :dabase.node-allocator :dabase.node-allocator '(:node-allocator)))

(defconstant +EMPTY+ 0)

#|
DA & NODE
|#
(defstruct da
  (nodes #() :type (simple-array node))
  (entries #() :type simple-vector)
  (alloca t :type node-allocator:node-allocator))

(defun init-da (entries)
  (make-da :nodes (make-array 64 :element-type 'node :initial-element +EMPTY+)
           :entries entries
           :alloca (node-allocator:make)))

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
  (let ((da (init-da entries)))
    (build-impl da 0 (length entries) 0)
    (output-to-file da output-file))
  :done)

(defun get-key (da index)
  (first (aref (da-entries da) index)))

(defun get-value (da index)
  (second (aref (da-entries da) index)))

(defun node-alloc (da arcs)
  (node-allocator:allocate (da-alloca da) arcs))

(defun (setf get-base) (value da index)
  (declare ((unsigned-byte 24) value))
  (setf (ldb (byte 24 0) (aref (da-nodes da) index)) value))

(defun set-node (da node-index base-node-index arc)
  (let ((next-index (+ base-node-index arc)))
    (setf (get-base da node-index) base-node-index
          (get-chck da node-index) arc

#|
BUILD
|#
(defun build-leaf (da entry-index node-index)
  (loop WITH key = (get-key da entry-index)
        UNTIL (octet-stream:eos? key)
        FOR arc = (octet-stream:read key)
    DO
    (setf node-index (set-node da node-index (node-alloc da `(,arc)) arc))
    FINALLY
    (setf node-index (set-node da node-index (node-alloc da `(,octet-stream:null-octet))
                                              octet-stream:null-octet)
          (get-base da node-index) (get-value da entry-index))))

(defun build-impl (da beg end root-node-index) 
  (declare (da da)
           (fixnum beg end)
           (node root-node-index))
  (if (= (- end beg) 1)
      (build-leaf da beg root-node-index)
    (progn
      )))

#|
OUTPUT
|#
(defun output-to-file (da output-file)
  (declare (ignore da output-file))
  )

(eval-when (:compile-toplevel :load-toplevel)
  (rename-package :dabase.octet-stream :dabase.octet-stream '())
  (rename-package :dabase.node-allocator :dabase.node-allocator '()))
