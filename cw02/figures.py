import cairo


def figure_translate(ctx, x, y):
    ctx.translate(x, y)


def figure_scale(ctx, x, y):
    ctx.scale(x, y)


def draw(ctx):
    # draw figure_1
    ctx.set_source_rgb(1.0, 0.0, 0.0)
    ctx.move_to(10, 10)
    ctx.line_to(50, 10)
    ctx.line_to(50, 100)
    ctx.line_to(90, 100)
    ctx.line_to(90, 10)
    ctx.line_to(130, 10)
    ctx.line_to(130, 150)
    ctx.line_to(10, 150)
    ctx.line_to(10, 10)
    ctx.close_path()
    ctx.fill()
    #    ctx.stroke()

    # draw figure_2
    ctx.set_source_rgb(0.0, 0.5, 0.5)
    ctx.move_to(150, 10)
    ctx.line_to(220, 80)
    ctx.line_to(290, 10)
    ctx.line_to(255, 140)
    ctx.line_to(220, 100)
    ctx.line_to(185, 140)
    ctx.close_path()
    ctx.fill()




if __name__ == '__main__':
    # raster 640 x 480 pikseli, zapisany potem na dysk w formacie PNG
    w = 640
    h = 480

    surf = cairo.ImageSurface(cairo.FORMAT_RGB24, w, h)
    # konieczna zmienna !!! poprzez przekazywanie cairo.Context(surf) za kazdym razem - nie dziala
    surface = cairo.Context(surf)

    # rysuje w lewej-gornej polowce
    draw(surface)

    # rysuje w lewej-dolnej polowce             # transformacje odbywaja sie wzgledem ostatnio wykonanego rysowania
    # figure_scale(surface, 0.5, 0.5)
    # figure_translate(surface, 0, h)
    figure_translate(surface, 0, h/2)
    draw(surface)

    # rysuje w prawej-dolnej polowce
    # figure_scale(surface, 0.5, 0.5)
    # figure_translate(surface, 2*w, 0)
    figure_translate(surface, w/2, 0)
    draw(surface)

    # rysuje w prawej-gornej polowce
    # figure_scale(surface, 0.5, 0.5)
    # figure_translate(surface, 0, -4*h)
    figure_translate(surface, 0, -h/2)
    draw(surface)

    surf.write_to_png("figures_2.png")