-- Mike Lattanzio's Little Chunk

-- Material         COLOUR__________  LIGHTING________  SHININESS
chunk = gr.texturedmaterial({ 0.2, 0.2, 0.2}, { 0.2, 0.2, 0.2}, 10, 9)

xjoint = {1,2,3};
yjoint = {1,2,3};

myroot = gr.node('root')

particle = gr.cube('particle') 
particle:set_material( chunk )
particle:scale( 0.5, 0.5, 0.5 ) 
myroot:add_child( particle )

return myroot
