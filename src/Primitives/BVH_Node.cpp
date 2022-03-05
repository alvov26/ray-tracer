//
// Created by Aleksandr Lvov on 2021-11-05.
//

#include "BVH_Node.h"
#include <algorithm>

std::optional<HitRecord> BVH_Node::intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const {
    if (!box_.doesIntersect(ray, min_dist, max_dist)) return {};

    auto left_hit = left_->intersect(ray, min_dist, max_dist);
    auto right_hit = right_->intersect(ray, min_dist, max_dist);
    if (!left_hit) return right_hit;
    if (!right_hit) return left_hit;
    return right_hit->distance <= left_hit->distance ? right_hit : left_hit;
}

std::optional<AABB> BVH_Node::boundingBox(FloatT /*time0*/, FloatT /*time1*/) const {
    return box_;
}

BVH_Node::BVH_Node(const HittableList& list, FloatT time0, FloatT time1) {
    auto objects = list.getObjects();
    auto node = BVH_Node(objects.begin(), objects.end(), time0, time1);
    left_ = node.left_;
    right_ = node.right_;
    box_ = node.box_;
}

auto getComparator(int axis) {
    return [axis](auto &lhs, auto &rhs) {
        auto box_left  = lhs->boundingBox(0, 0);
        auto box_right = rhs->boundingBox(0, 0);

        assert(box_left && box_right);
        return box_left->min()[axis] < box_right->min()[axis];
    };
}

BVH_Node::BVH_Node(BVH_Node::Iterator begin, BVH_Node::Iterator end, FloatT time0, FloatT time1) {
    auto comparator = getComparator(randomInt(0, 2));

    auto obj_count = end - begin;
    if (obj_count == 1) {
        left_ = right_ = *begin;
    }
    else if (obj_count == 2) {
        auto less = comparator(*begin, *(begin + 1));
        if (less) {
            left_  = *begin;
            right_ = *(begin + 1);
        }
        else {
            left_  = *(begin + 1);
            right_ = *begin;
        }
    }
    else {
        std::sort(begin, end, comparator);
        auto middle = begin + obj_count / 2;

        left_  = std::make_shared<BVH_Node>(begin, middle, time0, time1);
        right_ = std::make_shared<BVH_Node>(middle, end, time0, time1);
    }

    auto box_left  = left_->boundingBox(time0, time1);
    auto box_right = right_->boundingBox(time0, time1);

    assert(box_left && box_right);// "No bounding box in BVH constructor"
    box_ = surroundingBox(*box_left, *box_right);
}