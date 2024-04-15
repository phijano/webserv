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
body += "<li><a href=\"/cgi/calc.cgi\">Test CGI</a></li></ul>"
body += "<h1> CGI TEST, ADD 2 NUMBERS </h1>\n"
body += "<div id=\"upload\">"
body += "<div>"
body += "<form method=POST action=\"/cgi/calc.cgi\" enctype=\"multipart/form-data\">"
body += "<label for=\"name\">Number 1:</label>"
body += "<input name=\"Name\" size=20 maxlength=50>"
body += "<label for=\"name\">Number 2:</label>"
body += "<input name=\"Name\" size=20 maxlength=50>"
body += "<input type=submit class=\"button\" value=\"Submit\">"

if 'REQUEST_METHOD' in os.environ.keys():
    method = os.environ['REQUEST_METHOD']
    if method == "POST":  
        number1 = ""
        number2 = ""
        for index, line in enumerate(sys.stdin):
            if index == 3:
                number1 = line
            if index == 7:
                number2 = line
        if number1 == "\r\n" or number2 == "\r\n":
            body += "Introduce two numbers!!"
        else:
            number1 = number1[:-2]
            number2 = number2[:-2]
            try:
                body += number1 + " + " + number2 + " = " + str(int(number1) + int(number2))
            except ValueError as ex:
                try:
                    body += number1 + " + " + number2 + " = " + str(float(number1) + float(number2))
                except ValueError as ex:
                    body += "Introduce two numbers!!"
else:
    body += "Introduce two numbers!!"

body += "</div>"
body += "</div>"
body += "</body></html>"

print("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + str(len(body)) +  "\n\n" + body)

