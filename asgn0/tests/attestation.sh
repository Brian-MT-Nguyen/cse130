# Tests for split on a large text file (frankenstein by Shelly)
text="I successfully created and am using an Ubuntu 22.04 virtual machine."

diff <(echo $text) attestation.txt
