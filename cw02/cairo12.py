#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
Przykład demonstrujący mechanizm obsługi zdarzeń, konkretnie kliknięć myszką.
Dodatkowo wprowadzono klasę opakowującą dla elegancji cały kod (no, nie tylko
dla elegancji, pozwala to np. łatwo stworzyć więcej niż jedno okno).

Pierwsze kliknięcie zaznacza punkt początkowy, drugie rysuje linię do punktu
końcowego, trzecie wymazuje rysunek i pozwala zacząć zabawę od początku.
"""

import gi
gi.require_version("Gtk", "3.0")
gi.require_version("Gdk", "3.0")
from gi.repository import Gdk, Gtk
gi.require_foreign("cairo")

# Klasa Okno dziedziczy po klasie Gtk.Window.
#
class Okno(Gtk.Window):

    # W Pythonie konstruktor klasy nazywa się __init__.
    #
    def __init__(self):
        super(Okno, self).__init__()

        # W Pythonie nie trzeba wstępnie deklarować pól obiektu, wystarczy coś
        # spróbować do nich podstawić i to je automatycznie stworzy. None jest
        # odpowiednikiem pustego wskaźnika lub SQL-owego NULL, oznacza brak
        # wartości.
        #
        self.x1 = None
        self.y1 = None
        self.x2 = None
        self.y2 = None

        self.set_title("Okno GTK")
        self.set_default_size(400, 300)
        self.connect("destroy", Gtk.main_quit)
        da = Gtk.DrawingArea()
        da.set_size_request(200, 200)
        da.connect("draw", self.on_draw)
        da.set_events(Gdk.EventMask.BUTTON_PRESS_MASK)
        da.connect("button-press-event", self.on_button_press)
        self.add(da)
        self.show_all()

    # Dość często stosowaną konwencją jest nadawanie metodom-callbackom
    # wywoływanym w reakcji na zdarzenia nazw pochodzących od tych zdarzeń.
    # W przypadku zdarzenia o nazwie "draw" mamy więc metodę "on_draw".
    # Jak w poprzednich przykładach jako argumenty dostaje referencję do
    # wymagającego przerysowania DrawingArea oraz związany z tym obszarem
    # kontekst rysowania Cairo, ale ponieważ teraz jest to metoda, a nie
    # funkcja, to jako dodatkowy pierwszy argument przekazywana jest
    # referencja do naszego obiektu-okna.
    #
    def on_draw(self, widget, ctx):

        # Biblioteka GTK wypełnia obszar kolorem tła zanim wywoła naszą
        # metodę-callback, robienie tego drugi raz byłoby nadmiarowe.
        #
        # w = widget.get_allocated_width()
        # h = widget.get_allocated_height()
        # ctx.set_source_rgb(1.0, 1.0, 1.0)
        # ctx.rectangle(0, 0, w, h)
        # ctx.fill()

        if self.x1 is not None:
            ctx.arc(self.x1, self.y1, 8, 0.0, 6.283)    # ciut mniej niż 2π
            ctx.close_path()
            ctx.stroke()

        if self.x2 is not None:
            ctx.move_to(self.x1, self.y1)
            ctx.line_to(self.x2, self.y2)
            ctx.stroke()

    # Tu jako trzeci argument dostajemy obiekt reprezentujący zdarzenie. Ma on
    # m.in. pola z współrzędnymi kliknięcia wyrażonymi w układzie związanym
    # z DrawingArea, bo to właśnie do niego "podpięliśmy" poniższą metodę
    # obsługi zdarzeń.
    #
    def on_button_press(self, widget, event):

        # Lewy przycisk myszki ma numer 1, nie obsługuj zdarzenia jeśli
        # to nie on został naciśnięty.
        #
        if event.button != 1:
            return False

        if self.x1 is None:
            self.x1 = event.x
            self.y1 = event.y
        elif self.x2 is None:
            self.x2 = event.x
            self.y2 = event.y
        else:
            self.x1 = None
            self.y1 = None
            self.x2 = None
            self.y2 = None

        # Poproś bibliotekę GTK aby przy najbliższej okazji przerysowała
        # nasze DrawingArea.
        #
        widget.queue_draw()

        # Obsłużyliśmy zdarzenie, GTK już nic więcej nie musi z nim robić.
        #
        return True

# Inicjalizacja aplikacji sprowadza się teraz do stworzenia okna
# i uruchomienia pętli obsługi zdarzeń biblioteki GTK.
#
o = Okno()
Gtk.main()
