# HOW TO PEROFRM TESTS AND ADJUSTMENTS

## Syncing up
set in the code to have the clock start at a time that will occur in the near future and is easy to set, like ㍟㍙㍞㍘㍘ etc, then flash the code. At about one to two seconds before (㍘a㍛ to ㍘a㍞) the time you flashed, plug the arduino into power. 

actually displaying a close resenblance to the correct time is not strictly necessary, what is important is to record a short video of the arduino clock display next to a proper display (could be normal decimal clock display, just one that can show a few increments in the duration of the video so second precision is much prefered to minute)

This will let you go through the video slowly and choose a frame close to both clocks ticking up or convinient position to do math, this gives a timestamp of real time and arduino time to use as calibration. 

**Be sure to record the timing between ticks used in the arduino when recording the start time!**

Much later, mainly after it has noticably deviated from the proper time, do a second video and record a timestamp from a real clock and arduino clock.

assuming the start timestamp is at day 0 and the end timestamp is at day - how ever many full days passed - you calculate the end time minus the start time to get the delta, both in real seconds or ㍘a㍙days, as well as the ticks of the arduino. 

## Calculating the new delay number

The AMS (arduino milliseconds) to real millisecond rate should be the time the arduino thinks passed (the number of ticks times the number of times it ticked, divided by the number of real milliseconds that passed. 

Multiplying the AMS/ms ratio by 25/72 sec corresponding to ㍘a㍙days will give the new rate the arduino should be set to. If you calculated the real time in dozonal you simply don't multiply the denominator by that factor in the previous calculation for AMS/ms.

This should then be put into wolfram alpha or another program that can give a continued fraction representation of the ratio.

## getting viable approximations of the calculated delay factor

Use the program `fraction_calc.py` (re working the hardcoded values) to print out all the approximations of the real time factor and choose one that seems suitable. It prints out the fraction, the number from the continued fraction and the offset percent compared to the real one taking into account the integer part as well as the fraction part. Generally lines with large numbers in the middle column increase the complexity for very limitted gain so choosing the line right above it is preferable.

## Getting numerator as code

Once a fraction is chosen, plug it into wolfram alpha asking to convert it to base 2, for example 189/349 is:

    0.1000101010....

each digit corresponds to a power of 2 to check again a different remainder, for example:

    x&(2**1 - 1) == 0 or x&(2**5 - 1) == 1 or x&(2**7 -1 ) == 3 or ...

We can run this condition through the checker with this python code:

    v = [x for x in range(DENO) if ...]; len(v)

If `v` is too low add more checks from the code, if `v` is too high increasing the threshold of one of the checks can lower it, for example changing `x&31 == 1` to `x&31 == 17` removes one of the numbers that match.

Once the code that can get the intended fraction with relatively few checks with very fast bitwise comparisons it can be implemented on the arduino to get a more accurate clock.


# Tests





tests before 3rd were insufficiently recorded to be reliable

Unit AMS = one Arduino MilliSecond, the time unit used in the delays on the board.

Proper ticking rate is 25/72 seconds/tick

## Test 1

Run at 346 + 11/32 AMS/tick

june 12th, 13:01:15 to june 14th, 16:15:50 

= 2 days 3 hours 14 minutes 35 seconds = 184475 seconds


start display: ㍞㍞㍙㍞㍙
end display: ㍠㍙㍠㍞㍞

㍚.㍠㍙㍠㍞㍞
-.㍞㍞㍙㍞㍙

㍚.㍙㍟㍟㍘㍝ ticks that occured = 531509 ticks

531509 ticks * (346 + 11/32 )AMS/tick = AMS in test

25sec/72ticks * (AMS in test) / (seconds in test) = (346 + 8293511/17001216) AMS/tick

[346; 2, 20, 43, 157, 1, 7, 1, 2, 2]

reasonable approximation is 346 + 20/41

should run slow by 1.257552653478342 seconds over the course of 365 days based on decimal approximation

calculated AMS/ms = 0.99788491784117

## Test 2

Run at 346 + 20/41 AMS/tick

started June 14th, ㍢㍚㍞㍘a㍘ aligned within ㍘a㍙

measured June 17th, ㍡㍣㍞㍡㍣ (real time) and ㍡㍣㍟㍟㍞ on the device
9b776_12
real:
-㍘.㍢㍚㍞㍘a㍘
+㍛.㍡㍣㍞㍡a㍣
=㍛.㍘㍚㍣㍚a㍙

`39b69b_12 - 0a2600_12` = 751561 real 25/72sec

`39b776_12 - 0a2600_12` = 751446 ticks

`(39b776_12 - 0a2600_12)*(346+20/41)/(39b69b_12 - 0a2600_12)`

using 346 + 189/349 as new approximation, next time remember to do end time minus start time it doesn't work well mixing those up.
