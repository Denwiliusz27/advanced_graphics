/* ***************************************************************************

Program demonstrujący użycie biblioteki Cairo do rysowania na ekranie różnych
różności (to było jej oryginalne zastosowanie). Oczywiście obszar, na którym
pojawi się rysunek musi zawierać się w jakimś oknie, w przykładzie występuje
więc również biblioteka do tworzenia graficznych interfejsów użytkownika
pozwalająca tworzyć tzw. kanwy, czyli puste prostokąty wypełniane przez
aplikację rysunkiem tworzonym przy pomocy Cairo.

Konkretnie, mamy tu bibliotekę GTK (oficjalnie: GTK+) oraz jej kontrolkę typu
"drawing area". Kontrolka tworzona jest w main() razem z zawierającym ją oknem
a instrukcje coś na niej rysujące znajdują się w on_draw(). Nazwa tej drugiej
funkcji nie ma tak naprawdę znaczenia, ważne że jest ona tzw. callbackiem
wywoływanym w razie pojawienia się sygnału/zdarzenia "draw". GTK generuje ten
sygnał gdy konieczne jest przerysowanie zawartości kontrolki. Funkcja-callback
dostaje trzy argumenty: uchwyt do kontrolki, uchwyt do kontekstu graficznego
biblioteki Cairo, podany przy rejestrowaniu callbacku wskaźnik na dodatkowe
dane aplikacji (tutaj jest to NULL).

W przykładzie używamy dwóch z oficjalnych kolorów UJ: ciemnoniebieskiego,
który w przestrzeni RGB ma składowe (0, 81, 158), i żółtego (252, 201, 0).

Ten przykład należy tylko przejrzeć. Nie da się go skompilować w pracowni, bo
nie ma w niej zainstalowanego pakietu libgtk-3-dev. Chętni mogą oczywiście
zainstalować pakiety deweloperskie GTK na swoich domowych komputerach i tam
eksperymentować; przyda się wtedy "pkg-config --cflags --libs gtk+-3.0".

*************************************************************************** */

#include <gtk/gtk.h>

gboolean on_draw(GtkWidget * widget, cairo_t * ctx, gpointer data)
{
    int w = gtk_widget_get_allocated_width(widget);
    int h = gtk_widget_get_allocated_height(widget);

    // GTK konfiguruje układ współrzędnych przekazywanego nam kontekstu Cairo
    // tak, aby jednostką długości był piksel; (0, 0) jest w lewym górnym rogu
    // kanwy, a (w, h) w prawym dolnym

    cairo_set_source_rgb(ctx, 0.0, 81 / 255.0, 158 / 255.0);
    cairo_rectangle(ctx, 0, 0, w, h);
    cairo_fill(ctx);

    cairo_set_source_rgb(ctx, 252 / 255.0, 201 / 255.0, 0.0);
    cairo_set_line_width(ctx, 7.5);
    cairo_move_to(ctx, 15, 15);
    cairo_line_to(ctx, w / 2.0, h - 15);
    cairo_line_to(ctx, w - 15, 15);
    cairo_stroke(ctx);

    return FALSE;   // kontynuuj obsługę sygnału
}

void on_destroy(GtkWidget * widget, gpointer data)
{
    gtk_main_quit();
}

int main(int argc, char * argv[])
{
    gtk_init(&argc, &argv);

    // stwórz "obiekt" reprezentujący okno (w cudzysłowie, bo to C a nie C++)
    GtkWidget * win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // ustal nazwę i początkowy rozmiar okna
    gtk_window_set_title(GTK_WINDOW(win), "Okno GTK");
    gtk_window_set_default_size(GTK_WINDOW(win), 400, 300);
    // zarejestruj callback wywoływany po zamknięciu okna
    g_signal_connect(win, "destroy", G_CALLBACK(on_destroy), NULL);

    // stwórz "obiekt" reprezentujący kanwę
    GtkWidget * da = gtk_drawing_area_new();
    // ustal minimalny rozmiar kanwy
    gtk_widget_set_size_request(da, 200, 200);
    // wskaż funkcję odpowiedzialną za jej przerysowywanie
    g_signal_connect(da, "draw", G_CALLBACK(on_draw), NULL);
    // wstaw kanwę do powyżej stworzonego okna
    gtk_container_add(GTK_CONTAINER(win), da);

    // pokaż okno wraz z zawartością, uruchom pętlę zdarzeń GTK
    gtk_widget_show_all(win);
    gtk_main();

    return 0;
}
