#! /usr/local/bin/python3
import sys
import os

body = "<!DOCTYPE html><html lang=\"en\">"
body += "<head>"
body += "<meta charset=\"UTF-8\">"
body += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
body += "<title>Webserver</title>"
body += "<link rel=\"stylesheet\" href=\"/assets/styles/style.css\">"
body += "</head>"
body += "<body>"
body += "<ul><li><a href=\"/index.html\">Home</a></li>"
body += "<li><a href=\"/upload/upload.html\">Upload File</a></li>"
body += "<li><a href=\"/cgi/calc.py\">Test CGI</a></li></ul>"
body += "</body></html>"

number = int(body)

print("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + str(len(body)) +  "\n\n" + body)

