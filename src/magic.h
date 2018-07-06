/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@efn.org)				   *
*	    Gabrielle Taylor						   *
*	    Brian Moore (zump@rom.org)					   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

/*
 * Spell functions.
 * Defined in magic.c.
 */
//Sage
DECLARE_SPELL_FUN(	spell_rift	          );
DECLARE_SPELL_FUN(	spell_mana_rip        );
DECLARE_SPELL_FUN(	spell_transcendence   );
DECLARE_SPELL_FUN(  spell_shifting_colors );

DECLARE_SPELL_FUN(	spell_acidneedles	);
DECLARE_SPELL_FUN(	spell_null		);
DECLARE_SPELL_FUN(      spell_acid_rain         );
DECLARE_SPELL_FUN(	spell_acid_blast	);
DECLARE_SPELL_FUN(	spell_armor		);
DECLARE_SPELL_FUN(	spell_bless		);
DECLARE_SPELL_FUN(	spell_blindness		);
DECLARE_SPELL_FUN(	spell_burning_hands	);
DECLARE_SPELL_FUN(	spell_call_lightning	);
DECLARE_SPELL_FUN(      spell_calm		);
DECLARE_SPELL_FUN(      spell_cancellation	);
DECLARE_SPELL_FUN(	spell_cause_critical	);
DECLARE_SPELL_FUN(	spell_cause_light	);
DECLARE_SPELL_FUN(	spell_cause_serious	);
DECLARE_SPELL_FUN(	spell_change_sex	);
DECLARE_SPELL_FUN(      spell_blizzard          );
DECLARE_SPELL_FUN(      spell_icy_wind          );
DECLARE_SPELL_FUN(      spell_electric_storm    );
DECLARE_SPELL_FUN(      spell_chain_lightning   );
DECLARE_SPELL_FUN(	spell_charm_person	);
DECLARE_SPELL_FUN(	spell_chill_touch	);
DECLARE_SPELL_FUN(	spell_colour_spray	);
DECLARE_SPELL_FUN(	spell_continual_light	);
DECLARE_SPELL_FUN(	spell_control_weather	);
DECLARE_SPELL_FUN(	spell_create_food	);
DECLARE_SPELL_FUN(	spell_create_rose	);
DECLARE_SPELL_FUN(	spell_create_spring	);
DECLARE_SPELL_FUN(	spell_create_water	);
DECLARE_SPELL_FUN(	spell_cure_blindness	);
DECLARE_SPELL_FUN(	spell_cure_critical	);
DECLARE_SPELL_FUN(      spell_cure_disease	);
DECLARE_SPELL_FUN(	spell_cure_light	);
DECLARE_SPELL_FUN(	spell_cure_poison	);
DECLARE_SPELL_FUN(	spell_cure_serious	);
DECLARE_SPELL_FUN(	spell_curse		);
DECLARE_SPELL_FUN(      spell_demonfire		);
DECLARE_SPELL_FUN(	spell_detect_evil	);
DECLARE_SPELL_FUN(	spell_detect_good	);
DECLARE_SPELL_FUN(	spell_detect_hidden	);
DECLARE_SPELL_FUN(	spell_detect_invis	);
DECLARE_SPELL_FUN(	spell_detect_magic	);
DECLARE_SPELL_FUN(	spell_detect_poison	);
DECLARE_SPELL_FUN(	spell_dispel_evil	);
DECLARE_SPELL_FUN(      spell_dispel_good       );
DECLARE_SPELL_FUN(	spell_dispel_magic	);
DECLARE_SPELL_FUN(	spell_earthquake	);
DECLARE_SPELL_FUN(	spell_enchant_armor	);
DECLARE_SPELL_FUN(	spell_enchant_weapon	);
DECLARE_SPELL_FUN(	spell_energy_drain	);
DECLARE_SPELL_FUN(	spell_faerie_fire	);
DECLARE_SPELL_FUN(	spell_faerie_fog	);
DECLARE_SPELL_FUN(	spell_fireball		);
DECLARE_SPELL_FUN(	spell_fireproof		);
DECLARE_SPELL_FUN(	spell_flamestrike	);
DECLARE_SPELL_FUN(	spell_floating_disc	);
DECLARE_SPELL_FUN(	spell_fly		);
DECLARE_SPELL_FUN(      spell_frenzy		);
DECLARE_SPELL_FUN(	spell_gate		);
DECLARE_SPELL_FUN(	spell_giant_strength	);
DECLARE_SPELL_FUN(	spell_harm		);
DECLARE_SPELL_FUN(      spell_haste		);
DECLARE_SPELL_FUN(	spell_heal		);
DECLARE_SPELL_FUN(	spell_heat_metal	);
DECLARE_SPELL_FUN(      spell_holy_word		);
DECLARE_SPELL_FUN(	spell_identify		);
DECLARE_SPELL_FUN(	spell_infravision	);
DECLARE_SPELL_FUN(	spell_invis		);
DECLARE_SPELL_FUN(	spell_know_alignment	);
DECLARE_SPELL_FUN(	spell_lightning_bolt	);
DECLARE_SPELL_FUN(	spell_locate_object	);
DECLARE_SPELL_FUN(	spell_magic_missile	);
DECLARE_SPELL_FUN(      spell_mass_healing	);
DECLARE_SPELL_FUN(	spell_mass_invis	);
DECLARE_SPELL_FUN(	spell_nexus		);
DECLARE_SPELL_FUN(	spell_pass_door		);
DECLARE_SPELL_FUN(      spell_plague		);
DECLARE_SPELL_FUN(	spell_poison		);
DECLARE_SPELL_FUN(	spell_portal		);
DECLARE_SPELL_FUN(	spell_protection_evil	);
DECLARE_SPELL_FUN(	spell_protection_good	);
DECLARE_SPELL_FUN(	spell_ray_of_truth	);
DECLARE_SPELL_FUN(	spell_recharge		);
DECLARE_SPELL_FUN(	spell_refresh		);
DECLARE_SPELL_FUN(	spell_remove_curse	);
DECLARE_SPELL_FUN(	spell_sanctuary		);
DECLARE_SPELL_FUN(	spell_shocking_grasp	);
DECLARE_SPELL_FUN(	spell_shield		);
DECLARE_SPELL_FUN(	spell_sleep		);
DECLARE_SPELL_FUN(	spell_slow		);
DECLARE_SPELL_FUN(	spell_stone_skin	);
DECLARE_SPELL_FUN(	spell_summon		);
DECLARE_SPELL_FUN(	spell_teleport		);
DECLARE_SPELL_FUN(	spell_ventriloquate	);
DECLARE_SPELL_FUN(	spell_weaken		);
DECLARE_SPELL_FUN(	spell_word_of_recall	);
DECLARE_SPELL_FUN(	spell_acid_breath	);
DECLARE_SPELL_FUN(	spell_fire_breath	);
DECLARE_SPELL_FUN(	spell_frost_breath	);
DECLARE_SPELL_FUN(	spell_gas_breath	);
DECLARE_SPELL_FUN(	spell_lightning_breath	);
DECLARE_SPELL_FUN(	spell_general_purpose	);

/* New spells...these, as well as a lot of new skills, will need
 the area file 'skills.are' which contains default object and mobile
 templates for those skills/spells that require them.
 ....Ceran
*/
DECLARE_SPELL_FUN(	spell_shock_sphere	);
DECLARE_SPELL_FUN(	spell_grounding	);
DECLARE_SPELL_FUN(	spell_disintergrate	);
DECLARE_SPELL_FUN(	spell_transfer_object	);
DECLARE_SPELL_FUN(	spell_divine_touch	);
DECLARE_SPELL_FUN(	spell_cremate	);
DECLARE_SPELL_FUN(	spell_animate_dead	);
DECLARE_SPELL_FUN(	spell_earthbind	);
DECLARE_SPELL_FUN(	spell_concatenate	);
DECLARE_SPELL_FUN(	spell_earthfade	);
DECLARE_SPELL_FUN(	spell_earthmaw	);
DECLARE_SPELL_FUN(	spell_tsunami	);
DECLARE_SPELL_FUN(	spell_animate_object	);
DECLARE_SPELL_FUN(	spell_icelance	);
DECLARE_SPELL_FUN(	spell_frostbolt	);
DECLARE_SPELL_FUN(	spell_familiar	);
DECLARE_SPELL_FUN(	spell_forget	);
DECLARE_SPELL_FUN(	spell_atrophy	);
DECLARE_SPELL_FUN(	spell_fire_and_ice	);
DECLARE_SPELL_FUN(	spell_utter_heal	);
DECLARE_SPELL_FUN(	spell_wrath_of_purity	);
DECLARE_SPELL_FUN(	spell_lifeline	);
DECLARE_SPELL_FUN(	spell_lightshield	);
DECLARE_SPELL_FUN(	spell_shadowself	);
DECLARE_SPELL_FUN(	spell_wither	);
DECLARE_SPELL_FUN(	spell_shadowstrike	);
DECLARE_SPELL_FUN(	spell_nightwalk	);
DECLARE_SPELL_FUN(	spell_wrath	);
DECLARE_SPELL_FUN(	spell_dark_wrath	);
DECLARE_SPELL_FUN(	spell_true_sight	);
DECLARE_SPELL_FUN(	spell_guard_call	);
DECLARE_SPELL_FUN(	spell_iceshield	);
DECLARE_SPELL_FUN(	spell_fireshield	);
DECLARE_SPELL_FUN(	spell_heavenly_sceptre_fire	);
DECLARE_SPELL_FUN(	spell_heavenly_sceptre_frenzy	);
DECLARE_SPELL_FUN(	spell_firestream	);
DECLARE_SPELL_FUN(	spell_call_slaves	);
DECLARE_SPELL_FUN(	spell_lesser_golem	);
DECLARE_SPELL_FUN(	spell_greater_golem	);
DECLARE_SPELL_FUN(	spell_lifebane	);
DECLARE_SPELL_FUN(	spell_absorb	);
DECLARE_SPELL_FUN(	spell_acid_spit	);
DECLARE_SPELL_FUN(	spell_evil_eye	);
DECLARE_SPELL_FUN(	spell_unholy_fire	);
DECLARE_SPELL_FUN(	spell_haunting		);
DECLARE_SPELL_FUN(	spell_deathspell	);
DECLARE_SPELL_FUN(	spell_power_word_kill	);
DECLARE_SPELL_FUN(	spell_power_word_stun	);
//DECLARE_SPELL_FUN(  spell_psi_blast		);
DECLARE_SPELL_FUN(	spell_curse_weapon	);
DECLARE_SPELL_FUN(	spell_bless_weapon	);
DECLARE_SPELL_FUN(	spell_trace	);
DECLARE_SPELL_FUN(	spell_flame_scorch	);
DECLARE_SPELL_FUN(	spell_soulbind	);
DECLARE_SPELL_FUN(	spell_talk_to_dead	);
DECLARE_SPELL_FUN(	spell_globe_of_darkness	);
DECLARE_SPELL_FUN(	spell_consecrate	);
DECLARE_SPELL_FUN(	spell_timestop	);
DECLARE_SPELL_FUN(	spell_cone_of_cold	);
DECLARE_SPELL_FUN(	spell_protective_shield	);
DECLARE_SPELL_FUN(	spell_disenchant_armor	);
DECLARE_SPELL_FUN(	spell_disenchant_weapon	);
DECLARE_SPELL_FUN(	spell_windwall	);
DECLARE_SPELL_FUN(	spell_summon_air_elemental	);
DECLARE_SPELL_FUN(	spell_summon_fire_elemental	);
DECLARE_SPELL_FUN(	spell_summon_earth_elemental	);
DECLARE_SPELL_FUN(	spell_summon_water_elemental	);
DECLARE_SPELL_FUN(	spell_drain	);
DECLARE_SPELL_FUN(	spell_web	);
DECLARE_SPELL_FUN(	spell_restoration	);
DECLARE_SPELL_FUN(	spell_holy_fire	);
DECLARE_SPELL_FUN(	spell_blade_barrier	);
DECLARE_SPELL_FUN(	spell_fortitude	);
DECLARE_SPELL_FUN(	spell_prevent_healing	);
DECLARE_SPELL_FUN(	spell_spiritblade	);
DECLARE_SPELL_FUN(	spell_iceball	);
DECLARE_SPELL_FUN(	spell_hand_of_vengeance	);
DECLARE_SPELL_FUN(	spell_regenerate	);
DECLARE_SPELL_FUN(	spell_animate_skeleton	);
DECLARE_SPELL_FUN(	spell_badge	);
DECLARE_SPELL_FUN(	spell_alarm	);
DECLARE_SPELL_FUN(	spell_decoy	);
DECLARE_SPELL_FUN(	spell_revolt	);
DECLARE_SPELL_FUN(	spell_mummify	);
DECLARE_SPELL_FUN(	spell_preserve	);
DECLARE_SPELL_FUN(	spell_regeneration	);
DECLARE_SPELL_FUN(	spell_decay_corpse	);
DECLARE_SPELL_FUN(	spell_nightfall	);
DECLARE_SPELL_FUN(	spell_dark_shield	);
DECLARE_SPELL_FUN(	spell_power_word_fear	);
DECLARE_SPELL_FUN(	spell_brew	);
DECLARE_SPELL_FUN(	spell_team_spirit	);
DECLARE_SPELL_FUN(	spell_venueport	);
DECLARE_SPELL_FUN(	spell_channel	);
DECLARE_SPELL_FUN(	spell_life_transfer	);
DECLARE_SPELL_FUN(	spell_turn_undead	);
DECLARE_SPELL_FUN(	spell_protectorate_plate	);
DECLARE_SPELL_FUN(	spell_defiance	);
DECLARE_SPELL_FUN(	spell_undead_drain	);
DECLARE_SPELL_FUN(	spell_safety	);
DECLARE_SPELL_FUN(	spell_life_line	);
DECLARE_SPELL_FUN(      spell_hold_person ); /* Hold person -Detlef */
DECLARE_SPELL_FUN(      spell_slave_of_purgatory ); /* Spell for Ancient Only -Detlef */
DECLARE_SPELL_FUN(      spell_shroud    ); /* Spell for Ancient Only -Detlef */
DECLARE_SPELL_FUN(      spell_mental_knife    ); /* Spell for Kyr's tattoo -Dio */
DECLARE_SPELL_FUN(      spell_light_of_heaven ); /* Kizlatte quest spell -Kiz */
DECLARE_SPELL_FUN(	spell_scourge	); /* Scourge for Arcanas -Dev */
DECLARE_SPELL_FUN(	spell_kinetic_shield	); /* Kinetic Shield for Mages -Dev */
DECLARE_SPELL_FUN(      spell_garble            ); /* Garble for Outlaw -Detlef */
DECLARE_SPELL_FUN(	spell_wraithform	);
DECLARE_SPELL_FUN(	spell_unholy_bless	);
DECLARE_SPELL_FUN(	spell_crushing_hand	);
DECLARE_SPELL_FUN(      spell_eyes_of_intrigue  ); /* Eyes of Intrigue for Ancient -Detlef */
DECLARE_SPELL_FUN(		spell_darkforge			); // Darkforge for Ancient -- Ceial
DECLARE_SPELL_FUN(      spell_confuse           ); /* Confuse for Outlaw -Detlef */
DECLARE_SPELL_FUN(      spell_deafen            ); /* Deafen for AP -Detlef */
DECLARE_SPELL_FUN(      spell_scrying  );
DECLARE_SPELL_FUN(      spell_nova  );
DECLARE_SPELL_FUN(      spell_chaos_blade       );
DECLARE_SPELL_FUN(      spell_stalker           ); /* Stalker for Enforcer -Detlef */
DECLARE_SPELL_FUN(      spell_divine_aid           ); /* Quest Divine Aid -Dioxide */
DECLARE_SPELL_FUN(      spell_dream_blossom  );
DECLARE_SPELL_FUN(      spell_randomizer        );
DECLARE_SPELL_FUN(      spell_minion_tactics    ); /* Lich minion tactics -Dioxide */
DECLARE_SPELL_FUN(      spell_minion_sanc       ); /* Lich minion sanc -Dioxide */
DECLARE_SPELL_FUN(      spell_minion_flight     ); /* Lich minion flight -Dioxide */
DECLARE_SPELL_FUN(      spell_minion_sneak      ); /* Lich minion sneak -Dioxide */
DECLARE_SPELL_FUN(      spell_minion_recall     ); /* Lich minion recall -Dioxide */
DECLARE_SPELL_FUN(      spell_minion_haste      ); /* Lich minion haste -Dioxide */
DECLARE_SPELL_FUN(      spell_minion_trans      ); /* Lich minion passdoor -Dioxide */
DECLARE_SPELL_FUN(      spell_rejuvenate ); /* Cleric Rejuvenate -Dioxide */
DECLARE_SPELL_FUN(spell_spiderhands); /* Spiderhands -Graelik */
DECLARE_SPELL_FUN(	spell_graelik_tattoo	);
DECLARE_SPELL_FUN(spell_rot); /* Rot -Xanth */
DECLARE_SPELL_FUN(	spell_incinerate	); /* Dev */
DECLARE_SPELL_FUN(	spell_divine_aura		);
DECLARE_SPELL_FUN(      spell_pandemonium       ); /* Outlaw Skills */
DECLARE_SPELL_FUN(	spell_beast_call	);
/*a block of new elementalist spells*/
DECLARE_SPELL_FUN(      spell_flicker           );
DECLARE_SPELL_FUN(      spell_fissure           );
DECLARE_SPELL_FUN(      spell_self_immolation   );
DECLARE_SPELL_FUN(      spell_spontanious_combustion);
DECLARE_SPELL_FUN(      spell_shield_of_dust    );
DECLARE_SPELL_FUN(      spell_shield_of_frost   );
DECLARE_SPELL_FUN(      spell_shield_of_wind    );
DECLARE_SPELL_FUN(      spell_shield_of_lightning);
DECLARE_SPELL_FUN(      spell_shield_of_flames  );
DECLARE_SPELL_FUN(      spell_shield_of_bubbles );
DECLARE_SPELL_FUN(      spell_quake             );
DECLARE_SPELL_FUN(      spell_smoke             );
DECLARE_SPELL_FUN(      spell_pyrotechnics      );
DECLARE_SPELL_FUN(      spell_static_charge     );
DECLARE_SPELL_FUN(      spell_fireblast         );
DECLARE_SPELL_FUN(      spell_jolt              );
DECLARE_SPELL_FUN(      spell_ball_lightning    );
DECLARE_SPELL_FUN(      spell_ice_bolt          );
DECLARE_SPELL_FUN(      spell_winters_blast     );
DECLARE_SPELL_FUN(      spell_frost_blade       );
DECLARE_SPELL_FUN(      spell_freeze            );
DECLARE_SPELL_FUN(      spell_summon_zephyrus   );
DECLARE_SPELL_FUN(      spell_ice_slick         );
DECLARE_SPELL_FUN(      spell_create_vacume     );
DECLARE_SPELL_FUN(      spell_tornado           );
DECLARE_SPELL_FUN(      spell_sink_hole         );
DECLARE_SPELL_FUN(      spell_tunnel            );
DECLARE_SPELL_FUN(      spell_animate_earth     );
DECLARE_SPELL_FUN(      spell_flash_flood       );
DECLARE_SPELL_FUN(      spell_aqualung          );
DECLARE_SPELL_FUN(	spell_hydrolic_ram	);
DECLARE_SPELL_FUN(	spell_shockwave		);
DECLARE_SPELL_FUN(	spell_wind_walk		);
DECLARE_SPELL_FUN(      spell_vortex		);
/* spells for illusionists*/
DECLARE_SPELL_FUN(	spell_cloak_of_illusion	);
DECLARE_SPELL_FUN(      spell_mirror_images	);
DECLARE_SPELL_FUN(      spell_displacement	);
DECLARE_SPELL_FUN(      spell_pyschic_shroud	);
DECLARE_SPELL_FUN(      spell_mimic		);
DECLARE_SPELL_FUN(      spell_phantom		);
DECLARE_SPELL_FUN(      spell_doople		);
DECLARE_SPELL_FUN(		spell_divine_blessing );
DECLARE_SPELL_FUN(      spell_mask_sound	);
DECLARE_SPELL_FUN(      spell_nullify_sound	);
DECLARE_SPELL_FUN(      spell_create_sound	);
DECLARE_SPELL_FUN(      spell_project_illusion	);
DECLARE_SPELL_FUN(      spell_physchic_imprint	);
DECLARE_SPELL_FUN(      spell_detect_illusion	);
DECLARE_SPELL_FUN(      spell_dispel_illusion	);
DECLARE_SPELL_FUN(      spell_nightmare		);
DECLARE_SPELL_FUN(      spell_deadly_illusion   );
DECLARE_SPELL_FUN(	spell_hellfire		);
DECLARE_SPELL_FUN(      spell_black_shroud      );
DECLARE_SPELL_FUN(	spell_incandescense	);
DECLARE_SPELL_FUN(	spell_scorch		);
DECLARE_SPELL_FUN(	spell_looking_glass	);
DECLARE_SPELL_FUN(	spell_poison_flower	);
DECLARE_SPELL_FUN(	spell_wall_of_thorns	);
DECLARE_SPELL_FUN(	spell_barrier		);
DECLARE_SPELL_FUN(	spell_hex		);
DECLARE_SPELL_FUN(	spell_black_circle	);
DECLARE_SPELL_FUN(	spell_shield_onyx	);
DECLARE_SPELL_FUN(	spell_maehslin		);
DECLARE_SPELL_FUN(	spell_draal		);
DECLARE_SPELL_FUN(	spell_fatigue		);

DECLARE_SPELL_FUN(	spell_cure_deafness	);
DECLARE_SPELL_FUN(	spell_awaken		);
DECLARE_SPELL_FUN(	spell_resist_heat	);
DECLARE_SPELL_FUN(	spell_resist_cold	);
DECLARE_SPELL_FUN(	spell_resist_lightning	);
DECLARE_SPELL_FUN(	spell_resist_mental	);
DECLARE_SPELL_FUN(	spell_resist_acid	);
DECLARE_SPELL_FUN(	spell_resist_negative	);
DECLARE_SPELL_FUN(	spell_resist_paralysis	);
DECLARE_SPELL_FUN(	spell_resist_weapon	);
DECLARE_SPELL_FUN(	spell_remove_paralysis	);
DECLARE_SPELL_FUN(	spell_cleanse		);
DECLARE_SPELL_FUN(	spell_group_teleport	);
DECLARE_SPELL_FUN(	spell_strength		);
DECLARE_SPELL_FUN(	spell_soften		);
DECLARE_SPELL_FUN(	spell_duo_dimension	);
DECLARE_SPELL_FUN(	spell_protection	);
DECLARE_SPELL_FUN(	spell_prismatic_spray	);
DECLARE_SPELL_FUN(	spell_dragonweapon	);

// Air Elementalists

DECLARE_SPELL_FUN(	spell_air_shield	);
DECLARE_SPELL_FUN(	spell_turbulence	);
DECLARE_SPELL_FUN(	spell_mass_fly		);
DECLARE_SPELL_FUN(	spell_fog			);
DECLARE_SPELL_FUN(  spell_tornado		);

// Earth Elementalists

DECLARE_SPELL_FUN(	spell_landslide		);


