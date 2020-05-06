/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_KVM_PAGE_TRACK_H
#define _ASM_X86_KVM_PAGE_TRACK_H

enum kvm_page_track_mode {
	KVM_PAGE_TRACK_WRITE,
	KVM_PAGE_TRACK_MAX,
};


struct kvm_page_track_notifier_head {
	struct srcu_struct track_srcu;
	struct hlist_head track_notifier_list;
};

struct kvm_page_track_notifier_node {
	struct hlist_node node;

	
	void (*track_write)(struct kvm_vcpu *vcpu, gpa_t gpa, const u8 *new,
			    int bytes, struct kvm_page_track_notifier_node *node);
	
	void (*track_flush_slot)(struct kvm *kvm, struct kvm_memory_slot *slot,
			    struct kvm_page_track_notifier_node *node);
};

void kvm_page_track_init(struct kvm *kvm);
void kvm_page_track_cleanup(struct kvm *kvm);

void kvm_page_track_free_memslot(struct kvm_memory_slot *free,
				 struct kvm_memory_slot *dont);
int kvm_page_track_create_memslot(struct kvm_memory_slot *slot,
				  unsigned long npages);

void kvm_slot_page_track_add_page(struct kvm *kvm,
				  struct kvm_memory_slot *slot, gfn_t gfn,
				  enum kvm_page_track_mode mode);
void kvm_slot_page_track_remove_page(struct kvm *kvm,
				     struct kvm_memory_slot *slot, gfn_t gfn,
				     enum kvm_page_track_mode mode);
bool kvm_page_track_is_active(struct kvm_vcpu *vcpu, gfn_t gfn,
			      enum kvm_page_track_mode mode);

void
kvm_page_track_register_notifier(struct kvm *kvm,
				 struct kvm_page_track_notifier_node *n);
void
kvm_page_track_unregister_notifier(struct kvm *kvm,
				   struct kvm_page_track_notifier_node *n);
void kvm_page_track_write(struct kvm_vcpu *vcpu, gpa_t gpa, const u8 *new,
			  int bytes);
void kvm_page_track_flush_slot(struct kvm *kvm, struct kvm_memory_slot *slot);
#endif
