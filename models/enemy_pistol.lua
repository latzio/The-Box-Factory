-- Mike Lattanzio's Enemy model

myroot = gr.node('root') -- The root node of the pupppet

-- Material         COLOUR__________  LIGHTING________  SHININESS
shirt = gr.material({ 0.6, 0.2, 0.0}, { 0.2, 0.2, 0.2}, 20, 9)
pants = gr.texturedmaterial({ 0.4, 0.15, 0.075}, { 0.2, 0.2, 0.2}, 50, 10)
flesh = gr.material({ 0.2, 0.2, 0.2}, { 0.1, 0.1, 0.1}, 80)
shoes = gr.material({ 0.03, 0.03, 0.03}, { 0.4, 0.4, 0.4}, 40)
flesh = gr.material({ 0.03, 0.03, 0.03}, { 0.3, 0.3, 0.3}, 5)
soles = gr.material({ 0.0, 0.0, 0.0}, { 0.1, 0.1, 0.1}, 10)
buttn = gr.material({ 0.6, 0.6, 0.0}, { 0.8, 0.8, 0.8}, 20)
beard = gr.material({ 0.2, 0.1, 0.0}, { 0.1, 0.1, 0.1}, 10)
pupil = gr.material({ 0.6, 0.6, 0.6}, { 0.2, 0.2, 0.2}, 90)
sword = gr.material({ 0.7, 0.2, 0.2}, { 0.0, 0.3, 0.3}, 10)

brick = gr.texturedmaterial({ 1.0, 1.0, 1.0}, { 0.1, 0.1, 0.1}, 10, 1)

xjoint = {1,2,3};
yjoint = {1,2,3};

mycog = gr.joint('cog', xjoint, yjoint) -- The rotation node (centre of gravity)
myroot:add_child( mycog )

armcog = gr.joint('armcog', xjoint, yjoint) -- The rotation node for the upper bode
myroot:add_child( armcog )


-- Variables to simplify drawing
leg_bias    = 10;
leg_trans   = 1.6;
leg_width   = 0.7;
shin_width  = 0.6;
leg_height  = leg_trans * 1.25;
foot_width  = 0.7;
foot_height = 0.6;
foot_length = 1.2;

arm_bias    = 10;
bicep_girth = 0.6;
bicep_trans = 0.9;
bicep_length= 1.3;

arm_girth   = 0.4;
arm_trans   = 1.0;
arm_length  = 1.4;

neck_length = 1.0;
neck_girth  = 0.4;

head_trans  = 1.1;
head_radius = 0.8;

hand_trans  = 0.8;
hand_girth  = 0.3;
hand_width  = 0.4;
hand_height = 0.7;

-- THE JOINTS AND UNPICKABLES
-- UPPER BODY
-- Head
head = gr.joint('head', xjoint, yjoint)
armcog:add_child( head )
head:translate(0.0, 3.0 + neck_length, 0.0)

-- right shoulder joint
rshoulder = gr.joint('rshoulder', xjoint, yjoint)
armcog:add_child( rshoulder )
rshoulder:translate( 2.0, 3.0, 0.0 )
rshoulder:rotate( 'x', -arm_bias )

-- left shoulder joint
lshoulder = gr.joint('lshoulder', xjoint, yjoint)
armcog:add_child( lshoulder )
lshoulder:translate( -2.0, 3.0, 0.0 )
lshoulder:rotate( 'x', -arm_bias )

-- right elbow joint
relbow = gr.joint('relbow', xjoint, yjoint)
rshoulder:add_child( relbow )
relbow:translate( 0.0, -bicep_trans * 2, 0.0 )
relbow:rotate( 'x', arm_bias * 10 )

-- left elbow joint
lelbow = gr.joint('lelbow', xjoint, yjoint)
lshoulder:add_child( lelbow )
lelbow:translate( 0.0, -bicep_trans * 2, 0.0 )
lelbow:rotate( 'x', arm_bias * 2 )

-- right wrist joint
rwrist = gr.joint('rwrist', xjoint, yjoint)
relbow:add_child( rwrist )
rwrist:translate( 0.0, -bicep_trans * 2, 0.0 )
rwrist:rotate( 'x', arm_bias)

-- left wrist joint
lwrist = gr.joint('lwrist', xjoint, yjoint)
lelbow:add_child( lwrist )
lwrist:translate( 0.0, -bicep_trans * 2, 0.0 )
lwrist:rotate( 'x', arm_bias)

-- LOWER BODY
-- right hip joint
rhip = gr.joint('rhip', xjoint, yjoint)
mycog:add_child( rhip )
rhip:translate( 1.0, 0.0, 0.0 )
rhip:rotate( 'x', leg_bias )

-- left hip joint
lhip = gr.joint('lhip', xjoint, yjoint)
mycog:add_child( lhip )
lhip:translate( -1.0, 0.0, 0.0 )
lhip:rotate( 'x', leg_bias )

-- right knee joint
rknee = gr.joint('rknee', xjoint, yjoint)
rhip:add_child( rknee )
rknee:translate( 0.0, -leg_trans*2, 0.0 )
rknee:rotate( 'x', -leg_bias * 2 )

-- left knee joint
lknee = gr.joint('lknee', xjoint, yjoint)
lhip:add_child( lknee )
lknee:translate( 0.0, -leg_trans*2, 0.0 )
lknee:rotate( 'x', -leg_bias * 2 )

-- right ankle joint
rankle = gr.joint('rankle', xjoint, yjoint)
rknee:add_child( rankle )
rankle:translate( 0.0, -leg_trans * 1.6, 0.0 )
rankle:rotate( 'x', leg_bias )

-- left ankle joint
lankle = gr.joint('lankle', xjoint, yjoint)
lknee:add_child( lankle )
lankle:translate( 0.0, -leg_trans * 1.6, 0.0 )
lankle:rotate( 'x', leg_bias )

-- END JOINTS AND UNPICKABLES
--
--
-- GEOMETRY NODES!

-- The torso
torso = gr.cube('torso')
armcog:add_child( torso )
torso:set_material( pants )
torso:translate(0.0, 1.5, 0.0)
torso:scale(1.5, 2.5, 1.1)

-- Shoulder Blades
blades = gr.cube('blades')
armcog:add_child( blades )
blades:set_material( shirt )
blades:translate(0.0, 3.0, 0.0)
blades:scale(2.0, 0.8, 0.8)

-- Head
h = gr.cube('h')
head:add_child( h )
h:set_material( flesh )
h:translate(0.0, head_trans, 0.0)
h:scale(head_radius, head_radius*1.2, head_radius)

-- eye
eye = gr.cube('eye')
head:add_child( eye )
eye:set_material( pupil )
eye:translate( 0, head_trans, head_radius )
eye:scale( .4, .2, .15 )

-- Right bicep
rb = gr.cube('rb')
rshoulder:add_child( rb )
rb:set_material( flesh )
rb:translate(0.0, -bicep_trans, 0.0)
rb:scale(bicep_girth, bicep_length, bicep_girth)

-- Left bicep
lb = gr.cube('lb')
lshoulder:add_child( lb )
lb:set_material( flesh )
lb:translate(0.0, -bicep_trans, 0.0)
lb:scale(bicep_girth, bicep_length, bicep_girth)

-- Right forearm
ru = gr.cube('rb')
relbow:add_child( ru )
ru:set_material( flesh )
ru:translate(0.0, -arm_trans, 0.0)
ru:scale(arm_girth, arm_length, arm_girth)

-- Left forearm
lu = gr.cube('lb')
lelbow:add_child( lu )
lu:set_material( flesh )
lu:translate(0.0, -arm_trans, 0.0)
lu:scale(arm_girth, arm_length, arm_girth)

-- Right hand
rh = gr.cube('rh')
rwrist:add_child( rh )
rh:set_material( shirt )
rh:translate(0.0, -hand_trans, 0.0)
rh:scale(hand_girth, hand_height, hand_width)

	-- Right UZI -- Main piece
	ruzi = gr.cube('ruzi')
	rwrist:add_child( ruzi )
	ruzi:set_material( pants )
	ruzi:translate(0.0, -hand_trans * 2 , hand_trans * 0.75)
	ruzi:scale(hand_girth *.6 , hand_height * 1.5, hand_width *.6)
	-- Right UZI -- handle
	ruzi = gr.cube('ruzi_handle')
	rwrist:add_child( ruzi )
	ruzi:set_material( pants )
	ruzi:translate(0.0, -hand_trans, 0.0)
	ruzi:scale(hand_girth *.5 , hand_height *.3, hand_width )

-- Left hand
lh = gr.cube('lh')
lwrist:add_child( lh )
lh:set_material( shirt )
lh:translate(0.0, -hand_trans, 0.0)
lh:scale(hand_girth, hand_height, hand_width)

-- Right Thigh
rt = gr.cube('rt')
rhip:add_child( rt )
rt:set_material( flesh )
rt:translate(0.0, -leg_trans, 0.0)
rt:scale( leg_width, leg_height, leg_width)

-- Left Thigh
lt = gr.cube('lt')
lhip:add_child( lt )
lt:set_material( flesh )
lt:translate(0.0, -leg_trans, 0.0)
lt:scale( leg_width, leg_height, leg_width)

-- Right Shin
rs = gr.cube('rs')
rknee:add_child( rs )
rs:set_material( flesh )
rs:translate(0.0, -leg_trans, 0.0)
rs:scale( shin_width, leg_height * .9, shin_width)

-- Left shin
ls = gr.cube('ls')
lknee:add_child( ls )
ls:set_material( flesh )
ls:translate(0.0, -leg_trans, 0.0)
ls:scale( shin_width, leg_height * .9, shin_width)

-- Right foot
rf = gr.cube('rf')
rankle:add_child( rf )
rf:set_material( shirt )
rf:translate( 0.0, -foot_height, foot_length/2  )
rf:scale( foot_width, foot_height * .8, foot_length )

-- Left foot
lf = gr.cube('lf')
lankle:add_child( lf )
lf:set_material( shirt )
lf:translate( 0.0, -foot_height, foot_length/2  )
lf:scale( foot_width, foot_height *.8, foot_length )

return myroot
