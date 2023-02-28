import cairo


def figure_translate(ctx, x, y):
    ctx.translate(x, y)


def figure_scale(ctx, x, y):
    ctx.scale(x, y)


def draw_rectangle(ctx):
    ctx.set_source_rgb(1.0, 0.0, 0.0)
    # ctx.move_to(320, 100)
    ctx.move_to(w/2, 0)
    # ctx.line_to(550, 500)
    ctx.line_to(w, h)
    # ctx.line_to( 90, 500)
    ctx.line_to( 0, h)
    ctx.close_path()
    ctx.stroke()


def sierpinski(ctx, level ):
    if level == 0:
        ctx.set_line_width(10 + 1.5*level+1)
        draw_rectangle(ctx)

    else:
        ctx.save()

        figure_scale(ctx, 0.5, 0.5)

        # figura po lewej na dole
        figure_translate(ctx, 0, h)
        sierpinski(ctx, level - 1)

        #figura po prawej na dole
        figure_translate(ctx, w, 0)
        sierpinski(ctx, level - 1)

        # figura na gorze
        figure_translate(ctx, -w/2, -h)
        sierpinski(ctx, level - 1)

        ctx.restore()



if __name__ == '__main__':
    w = 640
    h = 640

    surf = cairo.ImageSurface(cairo.FORMAT_RGB24, w, h)
    # konieczna zmienna !!! poprzez przekazywanie cairo.Context(surf) za kazdym razem - nie dziala
    context = cairo.Context(surf)

    sierpinski(context, 5)
    surf.write_to_png("sierpinski.png")
