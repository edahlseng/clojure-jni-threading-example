(ns main
  (:import Test TestFunction)
  (:require [clojure.pprint]))

(defn -main []
  (let [a (byte-array 5)]
    (println "a" (alength a)))

  (Test/test
   (reify TestFunction
     (callMe [this]
       (let [b (byte-array 6)]
         (println "b" (alength b))))))

  (println "Done! Should exit momentarily..."))
