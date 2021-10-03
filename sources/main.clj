(ns main
  (:import Test TestFunction)
  (:require [clojure.pprint]))

(defn alength-wrapper [x]
  (alength x))

; Use this function instead of alength-wrapper to prevent exceptions during JNI calls from other threads
(defn alength-wrapper-with-hint [x]
  (alength ^bytes x))

(defn -main []
  (let [a (byte-array 5)]
    (println (Thread/currentThread))
    (println "a" (alength a)))

  @(future
     (let [b (byte-array 4)]
       (println (Thread/currentThread))
       (println "b" (alength-wrapper b))))

  (Test/test
   (reify TestFunction
     (callMe [this]
       (let [c (byte-array 6)]
         (println "c" (alength-wrapper c))))))

  (println "Done! Should exit momentarily..."))
