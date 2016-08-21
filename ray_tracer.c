/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_tracer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avella <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/27 17:41:04 by avella            #+#    #+#             */
/*   Updated: 2016/05/27 17:44:05 by avella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_h.h"

void	eye_pos_dir(t_env *e, double x, double y)
{
	double	cw;
	double	ch;

	cw = (e->width - x * 2) / e->height;
	ch = (e->height - y * 2) / e->height;
	e->ray_dir.x = -cos(e->v2) * sin(e->v)
		+ cw * cos(e->v) + ch * sin(e->v2) * sin(e->v);
	e->ray_dir.y = sin(e->v2) + ch * cos(e->v2);
	e->ray_dir.z = cos(e->v2) * cos(e->v)
		+ cw * sin(e->v) - ch * sin(e->v2) * cos(e->v);
	ajust(&(e->ray_dir));
	e->ray_dir.x = -e->ray_dir.x;
	e->ray_dir.z = -e->ray_dir.z;
	e->ray_origin = e->eye_pos;
	e->obj_tmp = NULL;
}

void	give_my_pos(t_env *e)
{
	e->pos.x = e->ray_origin.x + e->value * e->ray_dir.x;
	e->pos.y = e->ray_origin.y + e->value * e->ray_dir.y;
	e->pos.z = e->ray_origin.z + e->value * e->ray_dir.z;
	all_effect(e);
}

t_vec3d	final_color(t_env *e)
{
	e->value = 100000000;
	e->obj_tmp = all_inter(e);
	if (e->obj_tmp && e->value > 0.0001)
	{
		e->color = (t_vec3d){e->obj_tmp->color.x,
		e->obj_tmp->color.y, e->obj_tmp->color.z};
		if (e->value < 100000000)
			give_my_pos(e);
	}
	else
		e->color = (t_vec3d){0, 0, 0};
	return (e->color);
}

t_vec3d	give_color(t_env *e, double x, double y)
{
	eye_pos_dir(e, x, y);
	e->color = (t_vec3d){0, 0, 0};
	e->color = final_color(e);
	return (e->color);
}

void	ray_tracer(t_env *e)
{
	int x;
	int y;

	e->img = (t_img *)malloc(sizeof(t_img) * 1);
	e->img->img_ptr = mlx_new_image(e->mlx, e->width, e->height);
	y = 0;
	while (y < e->height)
	{
		x = 0;
		while (x < e->width)
		{
			e->color = give_color(e, x, y);
			pixel_put_to_img(e, x, y);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(e->mlx, e->win, e->img->img_ptr, 0, 0);
}
