echo "Status: 200\r"
echo "Content-Length: $CONTENT_LENGTH\r"
echo "Content-Type: text/plain\r\n\r"

# Read the body if size not 0
if [ $CONTENT_LENGTH -gt 0 ]; then
    head -c $CONTENT_LENGTH
fi
