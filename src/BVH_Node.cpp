//
// Created by Aleksandr Lvov on 2021-11-05.
//

#include "BVH_Node.h"
#include <algorithm>

std::optional<HitRecord> BVH_Node::intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const {
    if (!box.doesIntersect(ray, min_dist, max_dist)) return {};

    auto left_hit = left->intersect(ray, min_dist, max_dist);
    auto right_hit = right->intersect(ray, min_dist, left_hit ? left_hit->distance : max_dist);

    return right_hit ? right_hit : left_hit;
}

std::optional<AABB> BVH_Node::boundingBox(FloatT time0, FloatT time1) const {
    return box;
}

BVH_Node::BVH_Node(const HittableList &list, FloatT time0, FloatT time1)
    : BVH_Node(list.getObjects(), 0, list.getObjects().size(), time0, time1) {}

BVH_Node::BVH_Node(const std::vector<std::shared_ptr<Hittable>> &list,
                   size_t start, size_t end, FloatT time0, FloatT time1) {
    auto objects = list;
    auto comparator = [i = randomInt(0, 2)](auto &lhs, auto &rhs) {
        auto box_left = lhs->boundingBox(0, 0);
        auto box_right = rhs->boundingBox(0, 0);

        assert(box_left && box_right);
        return box_left->min()[i] < box_right->min()[i];
    };

    auto object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            left = objects[start];
            right = objects[start + 1];
        } else {
            left = objects[start + 1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);
        auto mid = start + object_span / 2;
        left = std::make_shared<BVH_Node>(objects, start, mid, time0, time1);
        right = std::make_shared<BVH_Node>(objects, mid, end, time0, time1);
    }

    auto box_left = left->boundingBox(time0, time1);
    auto box_right = right->boundingBox(time0, time1);

    assert(box_left && box_right);// "No bounding box in BVH constructor"
    box = surroundingBox(*box_left, *box_right);
}