#!/bin/sh

# Set Session Name
SESSION="OtelAdapter"
SESSIONEXISTS=$(tmux list-sessions | grep $SESSION)

# Only create tmux session if it doesn't already exist
if [ "$SESSIONEXISTS" = "" ]
then
    # Start New Session with our name
    tmux new-session -d -s $SESSION

    # Name first Pane and start the routing service
    tmux rename-window -t 0 'Routing Service'
    tmux send-keys -t 'Routing Service' 'rtiroutingservice -cfgFile ./RsTelemetryGateway.xml -cfgName "OpenTelemetryGateway"' C-m 'clear' C-m

    # Create and setup pane for generator
    tmux new-window -t $SESSION:1 -n 'Metrics Generator'
    tmux send-keys -t 'Metrics Generator' './generator -d 2 -q generator_qos.xml' C-m
fi

# Attach Session, on the Main window
tmux attach-session -t $SESSION:0
