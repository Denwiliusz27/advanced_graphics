#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
Animacja cząsteczki poruszającej się ruchem Browna. Przykład demonstruje
użycie mechanizmu timeoutów oraz obsługę klawiatury: spacja wstrzymuje
i ponownie uruchamia animację, strzałki przesuwają cząsteczkę, Esc pozwala
zakończyć działanie aplikacji.

Procedurę obsługi klawiatury podpinamy do okna, a nie do DrawingArea, bo
to od okna zaczyna się przetwarzanie naciśniętych klawiszy i schodzi w dół
drzewa widgetów do kontrolki mającej w danej chwili fokus klawiatury.
"""

import gi
gi.require_version("Gtk", "3.0")
gi.require_version("Gdk", "3.0")
from gi.repository import GLib, Gdk, Gtk
gi.require_foreign("cairo")

from math import pi
import random

class Okno(Gtk.Window):

    PARTICLE_RADIUS = 8
    MAX_MOVEMENT_PER_FRAME = 4

    def __init__(self):
        super(Okno, self).__init__()

        # początkowe współrzędne cząsteczki
        self.x = 50
        self.y = 50

        self.set_title("Okno GTK")
        self.set_default_size(400, 300)
        self.connect("destroy", Gtk.main_quit)
        self.connect("key-press-event", self.on_key_press)
        self.da = Gtk.DrawingArea()
        self.da.set_size_request(200, 200)
        self.da.connect("draw", self.on_draw)
        self.add(self.da)
        self.show_all()

        # uruchomienie animacji
        self.tid = None
        self.toggle_animation()

    # Aby uruchomić bądź wstrzymać animację trzeba stworzyć bądź usunąć tak
    # zwany "timeout", czyli zapis mówiący pętli zdarzeń GTK że ma obowiązek
    # wywoływać wskazaną procedurę w ustalonych odstępach czasu. Pierwszym
    # argumentem podawanym przy tworzeniu jest odstęp w milisekundach, drugim
    # referencja do procedury, jako trzeci można podać wartość, która będzie
    # przekazywana tej procedurze podczas jej wywoływania (nie jest to nam do
    # niczego potrzebne, więc podajemy None).
    #
    def toggle_animation(self):
        if self.tid is None:
            self.tid = GLib.timeout_add(100, self.on_timeout, None)
        else:
            GLib.source_remove(self.tid)
            self.tid = None

    def on_timeout(self, user_data):
        m = Okno.MAX_MOVEMENT_PER_FRAME
        self.x += random.randint(-m, m)
        self.y += random.randint(-m, m)
        self.da.queue_draw()
        # zwracana wartość ma znaczenie -- False oznaczałoby chęć zakończenia
        # animacji, timeout zostałby automatycznie usunięty
        return True

    def on_draw(self, widget, ctx):
        w = widget.get_allocated_width()
        h = widget.get_allocated_height()

        if self.x < Okno.PARTICLE_RADIUS:
            self.x = Okno.PARTICLE_RADIUS
        if self.x > w - Okno.PARTICLE_RADIUS:
            self.x = w - Okno.PARTICLE_RADIUS
        if self.y < Okno.PARTICLE_RADIUS:
            self.y = Okno.PARTICLE_RADIUS
        if self.y > h - Okno.PARTICLE_RADIUS:
            self.y = h - Okno.PARTICLE_RADIUS

        ctx.arc(self.x, self.y, Okno.PARTICLE_RADIUS, 0.0, 2*pi)
        ctx.close_path()
        ctx.fill()

    def on_key_press(self, widget, event):
        if event.keyval == Gdk.KEY_Left:
            self.x -= Okno.MAX_MOVEMENT_PER_FRAME
        elif event.keyval == Gdk.KEY_Right:
            self.x += Okno.MAX_MOVEMENT_PER_FRAME
        elif event.keyval == Gdk.KEY_Up:
            self.y -= Okno.MAX_MOVEMENT_PER_FRAME
        elif event.keyval == Gdk.KEY_Down:
            self.y += Okno.MAX_MOVEMENT_PER_FRAME
        elif event.keyval == Gdk.KEY_space:
            self.toggle_animation()
        elif event.keyval == Gdk.KEY_Escape:
            Gtk.main_quit()
        else:
            return False
        self.da.queue_draw()
        return True

o = Okno()
Gtk.main()
