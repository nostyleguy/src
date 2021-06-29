ALTER TABLE ship_objects 
ADD WEAPON_PROJECTILE_INDEX VARCHAR2(400);
update version_number set version_number=272, min_version_number=272;
