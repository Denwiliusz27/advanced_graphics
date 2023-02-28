#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
Pythonowa wersja przykładu pokazującego użycie Cairo w połączeniu z okienkową
biblioteką GTK. Jak poprzednio wspomniano, rasteryzacja wektorowych prymitywów
na ekranie komputera (z gwarancją uzyskania tych samych rezultatów niezależnie
od systemu operacyjnego) to podstawowe zastosowanie Cairo.

Ten skrypt powinien dać się uruchomić w wydziałowej pracowni komputerowej.
"""

import gi
gi.require_version("Gtk", "3.0")
from gi.repository import Gtk

# Ta funkcja będzie wywoływana jako callback za każdym razem gdy GTK uzna,
# że należy odświeżyć zawartość okna. Jako pierwszy argument przekazywana
# jest referencja do ekranowej kontrolki, którą trzeba przerysować; drugim
# argumentem jest kontekst Cairo odpowiadający jej powierzchni.
#
def rysuj(widget, ctx):
    w = widget.get_allocated_width()
    h = widget.get_allocated_height()

    ctx.set_source_rgb(0.0, 81 / 255.0, 158 / 255.0)
    ctx.rectangle(0, 0, w, h)
    ctx.fill()

    ctx.set_source_rgb(252 / 255.0, 201 / 255.0, 0.0)
    ctx.set_line_width(7.5)
    ctx.move_to(15, 15)
    ctx.line_to(w / 2.0, h - 15)
    ctx.line_to(w - 15, 15)
    ctx.stroke()

# Stwórz okno i wstawioną w nie kanwę, skonfiguruj, a potem uruchom
# główną pętlę obsługi zdarzeń biblioteki GTK.
#
win = Gtk.Window()
win.set_title("Okno GTK")
win.set_default_size(400, 300)
win.connect("destroy", Gtk.main_quit)
da = Gtk.DrawingArea()
da.set_size_request(200, 200)
da.connect("draw", rysuj)
win.add(da)
win.show_all()
Gtk.main()
