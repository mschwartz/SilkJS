/** @ignore */

function CanvasPathMethods(context) {
    this._context = context;
}
CanvasPathMethods.extend({
    // shared path API methods
    closePath: function() {
        cairo.context_close_path(this._context);
    },
    moveTo: function(x, y) {
        cairo.context_move_to(this._context, x, y);
    },
    lineTo: function(x, y) {
        cairo.context_line_to(this._context, x, y);
    },
    quadraticCurveTo: function(x1, y1, x2, y2) {
        var point = cairo.context_get_current_point(this._context);
        var x = point.x || x,
            y = point.y || y;
            
        cairo.context_curve_to(
            this._context, 
            x  + 2.0 / 3.0 * (x1 - x),  y  + 2.0 / 3.0 * (y1 - y),
            x2 + 2.0 / 3.0 * (x1 - x2), y2 + 2.0 / 3.0 * (y1 - y2), 
            x2,
            y2
        );
    },
    bezierCurveTo: function(cp1x, cp1y, cp2x, cp2y, x, y) {
        cairo.curve_to(this.context, cp1x, cp1y, cp2x, cp2y, x, y);
    },
    arcTo: function(x1, y1, x2, y2, radius) {
        var M_PI = Math.PI;
        
        // Current path point
        var ctx = this._context;
        var p0 = cairo.context_get_current_point(ctx),
            p1 = { x: x1, y: y1 },
            p2 = { x: x2, y: y2 };

        if ((p1.x == p0.x && p1.y == p0.y)
            || (p1.x == p2.x && p1.y == p2.y)
            || radius == 0) {
            cairo.context_line_to(ctx, p1.x, p1.y);
            return;
        }
        var p1p0 = { x: p0.x - p1.x, y: p0.y - p1.y },
            p1p2 = { x: p2.x - p1.x, y: p2.y - p1.y }
            p1p0_length = Math.sqrt(p1p0.x * p1p0.x + p1p0.y * p1p0.y),
            p1p2_length = Math.sqrt(p1p2.x * p1p2.x + p1p2.y * p1p2.y),
            cos_phi = (p1p0.x * p1p2.x + p1p0.y * p1p2.y) / (p1p0_length * p1p2_length);

        // all points on a line logic
        if (-1 == cos_phi) {
            cairo.context_line_to(ctx, p1.x, p1.y);
            return;
        }

        if (1 == cos_phi) {
            // add infinite far away point
            var max_length = 65535,
                factor_max = max_length / p1p0_length;
            cairo.context_line_to(ctx, p0.x + factor_max * p1p0.x, p0.y + factor_max * p1p0.y);
            return;
        }
        
        var tangent = radius / tan(Math.acos(cos_phi) / 2),
            factor_p1p0 = tangent / p1p0_length,
            t_p1p0 = {x: p1.x + factor_p1p0 * p1p0.x, y: p1.y + factor_p1p0 * p1p0.y };
            
        var orth_p1p0 = { x: p1p0.y, y: -p1p0.x},
            orth_p1p0_length = Math.sqrt(orth_p1p0.x * orth_p1p0.x + orth_p1p0.y * orth_p1p0.y),
            factor_ra = radius / orth_p1p0_length;
            
        var cos_alpha = (orth_p1p0.x * p1p2.x + orth_p1p0.y * p1p2.y) / (orth_p1p0_length * p1p2_length);
        if (cos_alpha < 0) {
            orth_p1p0 = { x: -orth_p1p0.x, y: -orth_p1p0.y};
        }

        var p = { x:t_p1p0.x + factor_ra * orth_p1p0.x, y: t_p1p0.y + factor_ra * orth_p1p0.y };

        orth_p1p0 = { x: -orth_p1p0.x, y: -orth_p1p0.y };
        var sa = Math.acos(orth_p1p0.x / orth_p1p0_length);
        if (orth_p1p0.y < 0) {
            sa = 2 * M_PI - sa;
        }

        var anticlockwise = false;

        var factor_p1p2 = tangent / p1p2_length,
            t_p1p2 = { x: p1.x + factor_p1p2 * p1p2.x, y: p1.y + factor_p1p2 * p1p2.y },
            orth_p1p2 = { x: t_p1p2.x - p.x, y: t_p1p2.y - p.y },
            orth_p1p2_length = Math.sqrt(orth_p1p2.x * orth_p1p2.x + orth_p1p2.y * orth_p1p2.y),
            ea = Math.acos(orth_p1p2.x / orth_p1p2_length);

        if (orth_p1p2.y < 0) {
            ea = 2 * M_PI - ea;
        }
        if ((sa > ea) && ((sa - ea) < M_PI)) {
            anticlockwise = true;
        }
        if ((sa < ea) && ((ea - sa) > M_PI)) {
            anticlockwise = true;
        }

        cairo.context_line_to(ctx, t_p1p0.x, t_p1p0.y);

        if (anticlockwise && M_PI * 2 != radius) {
            cairo.context_arc_negative(ctx
            , p.x
            , p.y
            , radius
            , sa
            , ea);
        } else {
            cairo.context_arc(ctx
            , p.x
            , p.y
            , radius
            , sa
            , ea);
        }        
    },
    rect: function(x,y, w, h) {
        cairo.context_rectangle(this._context, x, y, w, h);
    },
    arc: function(x,y, radius, startAngle, endAngle, anticlockwise) {
        if (anticlockwise && Math.PI * 2 != endAngle) {
            cairo.context_arc_negative(this._context, x, y, radius, startAngle, endAngle);
        }
        else {
            cairo.context_arc(this._context, x, y, radius, startAngle, endAngle);
        }
    }
});

