The position CSS property sets how an element is positioned in a document.
The `top`, `right`, `bottom`, and `left` physical properties and the
`inset-block-start`, `inset-block-end`, `inset-inline-start`, and
`inset-inline-end` flow-relative logical properties can be used to determine
the final location of positioned elements.


static
------

The element is positioned according to the Normal Flow of the document.
The top, right, bottom, left, and z-index properties have no effect.
This is the default value.

relative
--------

The element is positioned according to the normal flow of the document, and
then offset relative to itself based on the values of top, right, bottom,
and left.

The offset does not affect the position of any other elements; thus, the space
given for the element in the page layout is the same as if position were static.


sticky
------

The element is positioned according to the normal flow of the document, and then
offset relative to its nearest scrolling ancestor and containing block (nearest
block-level ancestor), including table-related elements, based on the values of
top, right, bottom, and left.


absolute
--------

The element is removed from the normal document flow, and no space is created
for the element in the page layout.


fixed
-----

The element is removed from the normal document flow, and no space is created
for the element in the page layout.

