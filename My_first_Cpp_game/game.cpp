#define is_down(b) input -> buttons[b].is_down
#define pressed(b) (input -> buttons[b].is_down && input -> buttons[b].changed)
#define released(b) (!input -> buttons[b].changed && input -> buttons[b].is_down)

float player_1_position, player_1_dp, player_2_position, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_pos_x, ball_pos_y, ball_dp_x = 120, ball_dp_y, ball_half_size;

int player_1_score, player_2_score;

internal void
simulate_player(float *p, float *dp, float ddp, float dt) {
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt * .5f;
	*dp = *dp + ddp * dt;

	if (*p + player_half_size_y > arena_half_size_y) {
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
	}
	else if (*p - player_half_size_y < -arena_half_size_y) {
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y, float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x&&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}

internal void Simulate_Game(Input* input, float dt) {
	clear_screen(0xff5500);
	draw_rectangle(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33);

	float player_1_ddp = 50.f;
#if 0
	if (is_down(BUTTON_UP)) player_1_ddp += 2000;
	if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000;
#else
	if (ball_pos_y > player_1_position + 2.f) player_1_ddp += 1500;
	if (ball_pos_y < player_1_position - 2.f) player_1_ddp -= 1500;
#endif

	float player_2_ddp = 50.f;
	if (is_down(BUTTON_W)) player_2_ddp += 2000;
	if (is_down(BUTTON_S)) player_2_ddp -= 2000;

	simulate_player(&player_1_position, &player_1_dp, player_1_ddp, dt);
	simulate_player(&player_2_position, &player_2_dp, player_2_ddp, dt);

	

	ball_pos_x += ball_dp_x * dt;
	ball_pos_y += ball_dp_y * dt;
	
	draw_rectangle(ball_pos_x, ball_pos_y, 1, 1, 0xffffff);
	// simulate ball
	{
		if (aabb_vs_aabb(ball_pos_x, ball_pos_y, ball_half_size, ball_half_size, 80, player_1_position, player_half_size_x, player_half_size_y)) {
			ball_pos_x = 80 - ball_half_size - player_half_size_x;
			ball_dp_x *= -1;
			ball_dp_y = (ball_dp_y - player_1_position) * 2 + player_1_dp * 1.25f;
		}
		else if (aabb_vs_aabb(ball_pos_x, ball_pos_y, ball_half_size, ball_half_size, -80, player_2_position, player_half_size_x, player_half_size_y)) {
			ball_pos_x = -80 + ball_half_size + player_half_size_x;
			ball_dp_x *= -1;
			ball_dp_y = (ball_dp_y - player_2_position) * 2 + player_2_dp * 1.25f;
		}

		if (ball_pos_y + ball_half_size > arena_half_size_y) {
			ball_pos_y = arena_half_size_y - ball_half_size;
			ball_dp_y *= -.75f;
		}
		else if (ball_pos_y - ball_half_size < -arena_half_size_y) {
			ball_pos_y = -arena_half_size_y + ball_half_size;
			ball_dp_y *= -.75f;
		}

		if (ball_pos_x + ball_half_size > arena_half_size_x) {
			ball_pos_x = ball_pos_y = 0;
			ball_dp_y = 0;
			ball_dp_x *= -1;
			player_1_score++;
		}
		else if (ball_pos_x - ball_half_size < -arena_half_size_x) {
			ball_pos_x = ball_pos_y = 0;
			ball_dp_y = 0;
			ball_dp_x *= -1;
			player_2_score++;
		}
	}

	draw_number(player_1_score, -10, 40, 1.f, 000000);
	draw_number(player_2_score, 10, 40, 1.f, 000000);

	draw_rectangle(80, player_1_position, player_half_size_x, player_half_size_y, 0xff0000);
	draw_rectangle(-80, player_2_position, player_half_size_x, player_half_size_y, 0xff0000);
}