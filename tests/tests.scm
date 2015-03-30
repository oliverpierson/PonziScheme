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

(define (pow x n)
  (if (= n 0)
    1
    (* x (pow x (- n 1)))))

(define (geometric-series r nterms)
  (define (iter sum k)
    (if (= k nterms)
      (+ sum (pow r k))
      (iter (+ sum (pow r k)) (+ k 1))))
  (iter 0.0 0))

(if (= (geometric-series 0.5 200) 2.0)
  (display 'geometric-series-test-passed)
  (display 'geometric-series-test-failed))

(define (forever) (forever))

(define (forever-test)
    (if (= 0 0)
      'done
      (forever)))

(forever-test)
(display 'forever-test-passed)

