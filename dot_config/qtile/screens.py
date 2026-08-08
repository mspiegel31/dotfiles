# pylint: disable=invalid-name

from libqtile.config import Screen 
from libqtile import widget, bar

screens = [
    Screen(
        bottom=bar.Bar(
            [
                widget.GroupBox(),
                widget.Prompt(),
                widget.WindowName(),
                widget.TextBox("Mike's config", name="default"),
                widget.Systray(),
                widget.Clock(format='%Y-%m-%d %a %I:%M %p'),
                widget.CurrentLayout(),
            ],
            24,
        ),
    ),
]