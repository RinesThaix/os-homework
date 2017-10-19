#!/usr/bin/env bash
read template
cat test.txt | grep -i $template >> result.txt