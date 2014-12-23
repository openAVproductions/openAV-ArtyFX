function(e) {
    if (!e.data.canvas) {
	// This is the first call, let's instantiate a canvas
	// Our canvas will occupy the whole <div class='graphics'> element, that is 150x130
	e.data.canvas = Raphael(e.icon.find('.graphics')[0], 150, 130)
    } else {
	// Let's remove the old graph so we can draw a new one now
	e.data.graph.remove()
    }
    // Draw a circle using the freq value to determine the radius, just as an example
    var graph = e.data.canvas.circle(75, 65, 90 * e.values.freq)
    graph.attr("fill", "#f00");
    graph.attr("stroke", "#fff");

    // Now store our graph in e.data, so that it will be available next call
    e.data.graph = graph
}
