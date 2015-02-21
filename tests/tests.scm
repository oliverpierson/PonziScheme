(define (fact n) 
    (define inc (lambda (x) (+ x 1)))
    (define (fact-iter count acc)
        (if (= count n)
          (* n acc)
          (fact-iter (inc count) (* count acc))))
    (fact-iter 1 1))

(if (= (fact 5) 120)
  (display 'fact-test-passed)
  (display 'fact-test-failed))

(define (forever) (forever))

(define (forever-test)
    (if (= 0 0)
      'done
      (forever)))

(forever-test)
(display 'forever-test-passed)

