# zeusd

## Introduction

This is the server utility of my Wifinanny program. The Wifinanny was an idea
I had when completing my first Master's at WGU. The idea is to implement
a transparent proxy on a wifi router, which captures the URL requests. This
is easily accomplished using Squid. Using this client as a squidhelper, we
can submit the url to a centralized database daemon (zeusd), which will then
check to see if the client is allowed to visit the url.

This server utility is nothing bit a C wrapper for a PHP script. The plan
was to eventually write a proper daemonized C utility, but I never got
to it.

## Installation

Installation is very simple. Just configure and compile. Make sure you have
PHP installed or it won't work.

## License

Ignore any headers regarding proprietary blah-blah. It's hereby released
as GPL. Cluecentral Inc was dissolved, and I was its president.
