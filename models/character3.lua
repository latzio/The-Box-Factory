-- Mike Lattanzio's Puppet

myroot = gr.node('root') -- The root node of the pupppet

-- Material         COLOUR__________  LIGHTING________  SHININESS
shirt = gr.material({ 0.5, 0.0, 0.5}, { 0.1, 0.2, 0.1}, 15)
flesh = gr.material({ 0.4, 0.2, 0.1}, { 0.1, 0.1, 0.1}, 80)
pants = gr.material({ 0.05, 0.05, 0.075}, { 0.2, 0.2, 0.2}, 10)
shoes = gr.material({ 0.03, 0.03, 0.03}, { 0.4, 0.4, 0.4}, 40)
soles = gr.material({ 0.0, 0.0, 0.0}, { 0.1, 0.1, 0.1}, 10)
buttn = gr.material({ 0.6, 0.6, 0.0}, { 0.8, 0.8, 0.8}, 20)
beard = gr.material({ 0.2, 0.1, 0.0}, { 0.1, 0.1, 0.1}, 10)
eye   = gr.material({ 0.0, 0.0, 0.0}, { 0.8, 0.8, 0.8}, 20)
metal = gr.texturedmaterial({ 0.3, 0.35, 0.3}, { 0.9, 0.9, 0.9}, 10, 9)
mag   = gr.texturedmaterial({ 0.3, 0.35, 0.3}, { 0.5, 0.5, 0.5}, 10, 9)

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
shin_width  = 0.7;
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
-- Neck Joint
neck = gr.joint('neck', xjoint, yjoint)
armcog:add_child( neck )
neck:translate(0.0, 3.0, 0.0)

head = gr.joint('head', xjoint, yjoint)
neck:add_child( head )
head:translate(0.0, neck_length, 0.0)

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
relbow:rotate( 'x', arm_bias * 2 )

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
torso = gr.sphere('torso')
armcog:add_child( torso )
torso:set_material( shirt )
torso:translate(0.0, 1.5, 0.0)
torso:scale(1.5, 2.5, 1.1)

-- Right suspender
rsus = gr.sphere('rsus')
armcog:add_child( rsus )
rsus:set_material( pants )
rsus:translate(0.8, 1.5, 0.0)
rsus:scale(0.3, 2.30, 1.0)

-- Right soulder suspender
rsh = gr.sphere('rsh')
armcog:add_child( rsh )
rsh:set_material( pants )
rsh:translate(0.8, 3.0, 0.0)
rsh:scale(0.3, 1.0, 1.0)

-- Right button
rbutton = gr.sphere('rbutton')
armcog:add_child( rbutton )
rbutton:set_material( buttn )
rbutton:translate(0.8, 0.6, 0.9)
rbutton:rotate('y', 40)
rbutton:scale(0.2, 0.2, 0.1)

-- Left suspender
lsus = gr.sphere('lsus')
armcog:add_child( lsus )
lsus:set_material( pants )
lsus:translate(-0.8, 1.5, 0.0)
lsus:scale(0.3, 2.30, 1.0)

-- Left soulder suspender
lsh = gr.sphere('lsh')
armcog:add_child( lsh )
lsh:set_material( pants )
lsh:translate(-0.8, 3.0, 0.0)
lsh:scale(0.3, 1.0, 1.0)

-- Left button
lbutton = gr.sphere('rbutton')
armcog:add_child( lbutton )
lbutton:set_material( buttn )
lbutton:translate(-0.8, 0.6, 0.9)
lbutton:rotate('y', -40)
lbutton:scale(0.2, 0.2, 0.1)

-- The Hips
hips = gr.sphere('hips')
mycog:add_child( hips )
hips:set_material( pants )
hips:translate(0.0, 0.5, 0.0)
hips:scale(1.4, 1.6, 1.2)

-- Shoulder Blades
blades = gr.sphere('blades')
armcog:add_child( blades )
blades:set_material( shirt )
blades:translate(0.0, 3.0, 0.0)
blades:scale(2.3, 1.0, 1.0)

-- Neck
n = gr.sphere('n')
neck:add_child( n )
n:set_material( flesh )
n:translate(0.0, neck_length, 0.0)
n:scale(neck_girth, neck_length , neck_girth)

-- Head
h = gr.sphere('h')
head:add_child( h )
h:set_material( flesh )
h:translate(0.0, head_trans, 0.0)
h:scale(head_radius, head_radius*1.2, head_radius)

-- hair
hair = gr.sphere('hair')
head:add_child( hair )
hair:set_material( beard )
hair:translate(0.0, head_trans +.08 , -0.08)
hair:scale(head_radius, head_radius*1.2, head_radius)

-- Hat
ha = gr.sphere('h')
head:add_child( ha )
ha:set_material( shirt )
ha:translate(0.0, head_trans * 1.60, -head_trans/9)
ha:rotate('x', 10)
ha:scale(head_radius, 0.6, head_radius)

-- Hat beak
hb = gr.sphere('h')
head:add_child( hb )
hb:set_material( shirt )
hb:translate(0.0, head_trans * 1.45, head_radius / 2)
hb:rotate('x', -5)
hb:scale(head_radius , 0.20, head_radius *.8 )

-- HAT L Bottom
hlb = gr.sphere('hlb')
head:add_child( hlb )
hlb:set_material( pants )
hlb:translate(0.05, head_trans * 1.65, head_radius * 3 / 4)
hlb:scale( .18, .07, .1 )

-- HAT L SIDE
hls = gr.sphere('hls')
head:add_child( hls )
hls:set_material( pants )
hls:translate( -0.055, head_trans * 1.65 +.12, head_radius * 3 / 4)
hlb:rotate('x', 5)
hls:scale( .07, .2, .1 )

-- NOSE
nose = gr.sphere('nose')
head:add_child( nose )
nose:set_material( flesh )
nose:translate( 0, head_trans - .3, head_radius*1.1 )
nose:scale( .3, .3, .3 )

-- Right stache
rstache = gr.sphere('rstache')
head:add_child( rstache )
rstache:set_material( beard )
rstache:translate( 0, head_trans, 0)
rstache:rotate( 'y', 20 )
rstache:rotate( 'x', -40 )
rstache:translate( 0, 0, head_radius)
rstache:rotate( 'z', -20 )
rstache:scale( .3, .15, .2)

-- Left stache
lstache = gr.sphere('lstache')
head:add_child( lstache )
lstache:set_material( beard )
lstache:translate( 0, head_trans, 0)
lstache:rotate( 'y', -20 )
lstache:rotate( 'x', -40 )
lstache:translate( 0, 0, head_radius)
lstache:rotate( 'z', 20 )
lstache:scale( .3, .15, .2)

-- Right eye
reye = gr.sphere('reye')
head:add_child( reye )
reye:set_material( eye )
reye:translate( 0, head_trans, 0 )
reye:rotate( 'y', 20 )
reye:rotate( 'x', 10 )
reye:translate( 0, 0, head_radius)
reye:scale( .085, .15, .05 )

-- Left eye
leye = gr.sphere('leye')
head:add_child( leye )
leye:set_material( eye )
leye:translate( 0, head_trans, 0 )
leye:rotate( 'y', -20 )
leye:rotate( 'x', 10 )
leye:translate( 0, 0, head_radius)
leye:scale( .085, .15, .05 )

-- Right bicep
rb = gr.sphere('rb')
rshoulder:add_child( rb )
rb:set_material( flesh )
rb:translate(0.0, -bicep_trans, 0.0)
rb:scale(bicep_girth, bicep_length, bicep_girth)

-- Left bicep
lb = gr.sphere('lb')
lshoulder:add_child( lb )
lb:set_material( flesh )
lb:translate(0.0, -bicep_trans, 0.0)
lb:scale(bicep_girth, bicep_length, bicep_girth)

-- Right forearm
ru = gr.sphere('rb')
relbow:add_child( ru )
ru:set_material( flesh )
ru:translate(0.0, -arm_trans, 0.0)
ru:scale(arm_girth, arm_length, arm_girth)

-- Left forearm
lu = gr.sphere('lb')
lelbow:add_child( lu )
lu:set_material( flesh )
lu:translate(0.0, -arm_trans, 0.0)
lu:scale(arm_girth, arm_length, arm_girth)

-- Right hand
rh = gr.sphere('rh')
rwrist:add_child( rh )
rh:set_material( flesh )
rh:translate(0.0, -hand_trans, 0.0)
rh:scale(hand_girth, hand_height, hand_width)

	-- Right UZI -- Main piece
	ruzi = gr.cube('ruzi')
	rwrist:add_child( ruzi )
	ruzi:set_material( metal )
	ruzi:translate(0.0, -hand_trans * 1.25 , hand_trans * 0.75)
	ruzi:scale(hand_girth *.6 , hand_height * 1.25, hand_width *.6)
	-- Right UZI -- handle
	ruzi = gr.cube('ruzi_handle')
	rwrist:add_child( ruzi )
	ruzi:set_material( metal )
	ruzi:translate(0.0, -hand_trans, 0.0)
	ruzi:scale(hand_girth *.5 , hand_height *.3, hand_width )
	-- Right UZI -- handle
	ruzi = gr.cube('ruzi_clip')
	rwrist:add_child( ruzi )
	ruzi:set_material( mag )
	ruzi:translate(0.0, -hand_trans, -hand_trans)
	ruzi:scale(hand_girth *.2 , hand_height *.2, hand_width )
	-- Right UZI -- nozzle
	ruzi = gr.cube('ruzi_nozzle')
	rwrist:add_child( ruzi )
	ruzi:set_material( metal )
	ruzi:translate(0.0, -hand_trans - hand_height, hand_trans * .75)
	ruzi:scale(hand_girth * .2, hand_height, hand_girth*.2)

	-- Left UZI -- Main piece
	luzi = gr.cube('luzi')
	lwrist:add_child( luzi )
	luzi:set_material( metal )
	luzi:translate(0.0, -hand_trans * 1.25 , hand_trans * 0.75)
	luzi:scale(hand_girth *.6 , hand_height * 1.25, hand_width *.6)
	-- Left UZI -- handle
	luzi = gr.cube('luzi_handle')
	lwrist:add_child( luzi )
	luzi:set_material( metal )
	luzi:translate(0.0, -hand_trans, 0.0)
	luzi:scale(hand_girth *.5 , hand_height *.3, hand_width )
	-- Left UZI -- handle
	luzi = gr.cube('luzi_clip')
	lwrist:add_child( luzi )
	luzi:set_material( mag )
	luzi:translate(0.0, -hand_trans, -hand_trans)
	luzi:scale(hand_girth *.2 , hand_height *.2, hand_width )
	-- Left UZI -- nozzle
	luzi = gr.cube('luzi_nozzle')
	lwrist:add_child( luzi )
	luzi:set_material( metal )
	luzi:translate(0.0, -hand_trans - hand_height, hand_trans * .75)
	luzi:scale(hand_girth * .2, hand_height, hand_girth*.2)

-- Left hand
lh = gr.sphere('lh')
lwrist:add_child( lh )
lh:set_material( flesh )
lh:translate(0.0, -hand_trans, 0.0)
lh:scale(hand_girth, hand_height, hand_width)

-- Right Thigh
rt = gr.sphere('rt')
rhip:add_child( rt )
rt:set_material( pants )
rt:translate(0.0, -leg_trans, 0.0)
rt:scale( leg_width, leg_height, leg_width)

-- Left Thigh
lt = gr.sphere('lt')
lhip:add_child( lt )
lt:set_material( pants )
lt:translate(0.0, -leg_trans, 0.0)
lt:scale( leg_width, leg_height, leg_width)

-- Right Shin
rs = gr.sphere('rs')
rknee:add_child( rs )
rs:set_material( pants )
rs:translate(0.0, -leg_trans, 0.0)
rs:scale( shin_width, leg_height, shin_width)

-- Left shin
ls = gr.sphere('ls')
lknee:add_child( ls )
ls:set_material( pants )
ls:translate(0.0, -leg_trans, 0.0)
ls:scale( leg_width, leg_height, leg_width)

-- Right foot
rf = gr.sphere('rf')
rankle:add_child( rf )
rf:set_material( shoes )
rf:translate( 0.0, -foot_height, foot_length/2  )
rf:scale( foot_width, foot_height * .8, foot_length )

-- Right sole
rsole = gr.sphere('rsole')
rankle:add_child( rsole )
rsole:set_material( soles )
rsole:translate( 0.0, -foot_height *1.4, foot_length/2  )
rsole:scale( foot_width, foot_height/2, foot_length )

-- Left foot
lf = gr.sphere('lf')
lankle:add_child( lf )
lf:set_material( shoes )
lf:translate( 0.0, -foot_height, foot_length/2  )
lf:scale( foot_width, foot_height *.8, foot_length )

-- Left sole
lsole = gr.sphere('lsole')
lankle:add_child( lsole )
lsole:set_material( soles )
lsole:translate( 0.0, -foot_height *1.4, foot_length/2  )
lsole:scale( foot_width, foot_height/2, foot_length )
return myroot
