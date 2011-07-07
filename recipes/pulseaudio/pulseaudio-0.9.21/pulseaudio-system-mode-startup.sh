#!/bin/sh

echo "Starting Pulseaudio in system mode.."
pulseaudio --log-target=syslog --system
