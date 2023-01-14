# test to see that hello produces reasonable output:
text="Hello World"

diff -i <(echo "Hello World!") <(./hello)
