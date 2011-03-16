-- Mike Lattanzio's Little Chunk

-- Material         COLOUR__________  LIGHTING________  SHININESS
sword = gr.material({ 0.7, 0.4, 0.0}, { 0.0, 0.3, 0.3}, 10)

hand_trans  = 0.8;
hand_girth  = 0.3;
hand_width  = 0.4;
hand_height = 0.7;
xjoint = {1,2,3};
yjoint = {1,2,3};

-- math.randomseed(os.time())
zrot = 0

myroot = gr.node('root')

math.randomseed( os.time() )

cog = gr.node('cog')
cog:rotate( 'y', math.random(0, 360) )
cog:rotate( 'z', 90 )
myroot:add_child( cog )

  -- Right Weapon -- Main piece
	ruzi = gr.cube('weapon')
	cog:add_child( ruzi )
	ruzi:set_material( sword )
	ruzi:translate(0.0, -hand_trans , hand_trans * 0.75)
	ruzi:scale(hand_girth *.3 , hand_height , hand_width *.3)

	-- Right Weaon -- blade
	ruzi = gr.cube('blade')
	cog:add_child( ruzi )
	ruzi:set_material( sword )
	ruzi:translate(0.0, -hand_trans, hand_trans * 2)
	ruzi:scale(hand_girth *.3 , hand_height *.4, hand_height * 4 )

return myroot
