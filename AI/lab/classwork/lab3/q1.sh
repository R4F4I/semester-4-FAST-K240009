#!/bin/bash

# Check if exactly 4 arguments are provided
if [ $# -ne 4 ]; then
    echo "Usage: $0 num1 num2 num3 num4"
    exit 1
fi

# Calculate the product
PRODUCT=$(($1 * $2 * $3 * $4))

# Print the result
echo "The product of $1, $2, $3 and $4 is $PRODUCT"