#include <string>
const std::string login_html = "<!DOCTYPE html>"
                              "<html>"
                                  "<body>"
                                      "<h1>Login</h1>"
                                      "<form action=\"/login.html\" method=\"post\">"
                                          "Username:<br>"
                                          "<input type=\"text\" name=\"username\"><br>"
                                          "Password:<br>"
                                          "<input type=\"password\" name=\"password\"><br><br>"
                                          "<input type=\"submit\" value=\"Submit\">"
                                      "</form>"
                                  "</body>"
                              "</html>";

const std::string hello_html = "<!DOCTYPE html>"
                              "<html>"
                                  "<body>"
                                      "<h1>Welcome</h1>"
                                      "Nice to see you {username}"
                                  "</body>"
                              "</html>";