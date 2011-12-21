(in-package :asdf)

(defsystem dabase
  :name "dabase"
  :version "0.0.1"
  :author "Takeru Ohta"
  :description "Basic Double Array"

  :serial t
  
  :components ((:file "package")
               (:file "node-allocator")
               (:file "builder")
               (:file "dabase")))
