#!/bin/sh
xev -1 -event keyboard | awk '/^KeyRelease/ {print $20}'
