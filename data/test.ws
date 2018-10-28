#!/usr/bin/env ws


config change_focus_to;


let desktop_num = 6;
let tally = 0;


let active_monitors = run "xrandr --query"
                   |> run "grep \"connected\""
                   |> run "cut -d' ' -f1";


let current_focused_monitor = run "bspc query -M -m focused --names";


for (m: active_monitors) {
    if (m == current_focused_monitor) {
        let focus_target = tally * desktop_num + change_focus_to;
        run "bspc desktop -f {}".format(focus_target);
        exit 0;
    }

    tally += 1;
}