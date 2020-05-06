

#ifndef _MLX5_FS_HELPERS_
#define _MLX5_FS_HELPERS_

#include <linux/mlx5/mlx5_ifc.h>

#define MLX5_FS_IPV4_VERSION 4
#define MLX5_FS_IPV6_VERSION 6

static inline bool mlx5_fs_is_ipsec_flow(const u32 *match_c)
{
	void *misc_params_c = MLX5_ADDR_OF(fte_match_param, match_c,
					   misc_parameters);

	return MLX5_GET(fte_match_set_misc, misc_params_c, outer_esp_spi);
}

static inline bool _mlx5_fs_is_outer_ipproto_flow(const u32 *match_c,
						  const u32 *match_v, u8 match)
{
	const void *headers_c = MLX5_ADDR_OF(fte_match_param, match_c,
					     outer_headers);
	const void *headers_v = MLX5_ADDR_OF(fte_match_param, match_v,
					     outer_headers);

	return MLX5_GET(fte_match_set_lyr_2_4, headers_c, ip_protocol) == 0xff &&
		MLX5_GET(fte_match_set_lyr_2_4, headers_v, ip_protocol) == match;
}

static inline bool mlx5_fs_is_outer_tcp_flow(const u32 *match_c,
					     const u32 *match_v)
{
	return _mlx5_fs_is_outer_ipproto_flow(match_c, match_v, IPPROTO_TCP);
}

static inline bool mlx5_fs_is_outer_udp_flow(const u32 *match_c,
					     const u32 *match_v)
{
	return _mlx5_fs_is_outer_ipproto_flow(match_c, match_v, IPPROTO_UDP);
}

static inline bool mlx5_fs_is_vxlan_flow(const u32 *match_c)
{
	void *misc_params_c = MLX5_ADDR_OF(fte_match_param, match_c,
					   misc_parameters);

	return MLX5_GET(fte_match_set_misc, misc_params_c, vxlan_vni);
}

static inline bool _mlx5_fs_is_outer_ipv_flow(struct mlx5_core_dev *mdev,
					      const u32 *match_c,
					      const u32 *match_v, int version)
{
	int match_ipv = MLX5_CAP_FLOWTABLE_NIC_RX(mdev,
						  ft_field_support.outer_ip_version);
	const void *headers_c = MLX5_ADDR_OF(fte_match_param, match_c,
					     outer_headers);
	const void *headers_v = MLX5_ADDR_OF(fte_match_param, match_v,
					     outer_headers);

	if (!match_ipv) {
		u16 ethertype;

		switch (version) {
		case MLX5_FS_IPV4_VERSION:
			ethertype = ETH_P_IP;
			break;
		case MLX5_FS_IPV6_VERSION:
			ethertype = ETH_P_IPV6;
			break;
		default:
			return false;
		}

		return MLX5_GET(fte_match_set_lyr_2_4, headers_c,
				ethertype) == 0xffff &&
			MLX5_GET(fte_match_set_lyr_2_4, headers_v,
				 ethertype) == ethertype;
	}

	return MLX5_GET(fte_match_set_lyr_2_4, headers_c,
			ip_version) == 0xf &&
		MLX5_GET(fte_match_set_lyr_2_4, headers_v,
			 ip_version) == version;
}

static inline bool
mlx5_fs_is_outer_ipv4_flow(struct mlx5_core_dev *mdev, const u32 *match_c,
			   const u32 *match_v)
{
	return _mlx5_fs_is_outer_ipv_flow(mdev, match_c, match_v,
					  MLX5_FS_IPV4_VERSION);
}

static inline bool
mlx5_fs_is_outer_ipv6_flow(struct mlx5_core_dev *mdev, const u32 *match_c,
			   const u32 *match_v)
{
	return _mlx5_fs_is_outer_ipv_flow(mdev, match_c, match_v,
					  MLX5_FS_IPV6_VERSION);
}

static inline bool mlx5_fs_is_outer_ipsec_flow(const u32 *match_c)
{
	void *misc_params_c =
			MLX5_ADDR_OF(fte_match_param, match_c, misc_parameters);

	return MLX5_GET(fte_match_set_misc, misc_params_c, outer_esp_spi);
}

#endif
