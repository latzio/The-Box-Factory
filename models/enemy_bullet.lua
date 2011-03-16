-- Mike Lattanzio's Bullet

shell = gr.material({ 1.0, 0.0, 0.0}, { 1.0, 1.0, 1.0}, 5)

xjoint = {1,1,1};
yjoint = {1,1,1};

-- Size and shape
caliber = .75
length =  .75

myroot = gr.node('root')

cog = gr.joint('cog', xjoint, yjoint)
myroot:add_child( cog )

travel = gr.node('travel')
cog:add_child(travel)

bullet = gr.sphere('shell', xjoint, yjoint) -- The rotation node (centre of gravity)
bullet:set_material( shell )
bullet:scale( caliber, caliber, length )
travel:add_child( bullet )

return myroot

