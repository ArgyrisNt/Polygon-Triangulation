# Polygon triangulation
A polygon triangulation method according to "Mark De Berg: Computational Geometry, Algorithms and Applications (3rd edition)" book.
First, you decompose the polygon into monotone pieces and then triangulate each of them. 
In order to save info of polygon edges, vertices and faces, I used a Doubly-Connected Edge List (DCEL) with half edges.


# Initial polygon

<img src="polygon.png" height="400" width="400" >


# Decomposition of polygon into monotone pieces

<img src="monotonePieces.png" height="400" width="400" >


# Triangulation of each monotone piece

<img src="triangulation.png" height="400" width="400" >
