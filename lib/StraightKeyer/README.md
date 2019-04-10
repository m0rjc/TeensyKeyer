Straight Keyer implementation
----

Initial version of the Straight Keyer would just generate a side tone. This allows me to use the
device as a practice oscillator with a straight key.

It should be quite plausible to drive the MorseDecoder using a straight key. A future StraightKeyer
module could be designed to work this way. I expect I'd find that measuring input tone lengths (and perhaps
gap lengths) will produce two buckets. I can then use a Baysian Discriminator to work out if I have a dot
or a dash. I adjust the decision point depending on input. A special case would be only dits arriving (words
like "is" "his") This special case could be detected.
