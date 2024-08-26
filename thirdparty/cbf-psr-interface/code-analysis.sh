#!/usr/bin/env bash
echo "STATIC CODE ANALYSIS"
echo "===================="
echo

echo "MODULE ANALYSIS"
echo "---------------"
pylint LowCbfMaster
#pylint LowCbfSubarray
#pylint LowCbfCapLogicalStation
#pylint LowCbfCapSearchBeam
#pylint LowCbfCapTimingBeam
#pylint GeminiDevice

echo "TESTS ANALYSIS"
echo "--------------"
pylint tests
